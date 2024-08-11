using Rhino;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Runtime;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;

using static System.ActivationContext.ContextForm;

namespace GridMaker
{
    public struct Grid {
        public Point3d Point0;
        public int Xcount, Ycount;
        public double Xrange, Yrange;
        public double ModuleSize;
        public Transform DeRotate;
        public Grid(Point3d point0, double module_size) {
            Point0 = point0;
            ModuleSize = module_size;
            DeRotate = Transform.Rotation(0, new Point3d(0, 0, 0));
            Xcount = Ycount = 0;
            Xrange = Yrange = 0;
        }
    }
    public struct CombinedInteriorMatrix {
        public int Width, Height;
        public int[,] Data;
    }
    public class GridMaker
    {
        public static CombinedInteriorMatrix MakeCombinedInteriorMatrixWithCurvesRotateModuleSize
            (Curve Interior, Curve Exterior, Curve Access, double module_size, double rotation) {
            Curve interior = Interior.DuplicateCurve();
            Curve exterior = Exterior.DuplicateCurve();
            Curve access = Access.DuplicateCurve();
            Grid grid = MakeGrid(new List<Curve> { interior, exterior, access }, module_size, rotation);
            int[,] insideInterior = MakeInteriorMatrix(grid, interior);
            int[,] insideExterior = MakeInteriorMatrix(grid, exterior);
            int[,] insideAccess = MakeInteriorMatrix(grid, access);
            CombinedInteriorMatrix bfao = MakeCombinedInteriorMatrix(insideInterior, insideExterior, insideAccess, grid);
            return bfao;
        }

        public static bool CheckCurve(Curve curve)
        {
            if (curve == null) return false;
            if (curve.IsClosed == false) return false;
            if (!curve.IsPlanar()) return false;
            if (curve.GetBoundingBox(true).IsDegenerate(-1) != 1) return false;
            if (curve.GetBoundingBox(true).Diagonal.Z != 0) return false;

            return true;
        }
        private static Grid MakeGrid(List<Curve> curves, double module_size, double rotation) {
            // Make Grid, and rotate the curves
            Transform transform_plus = Transform.Rotation(RhinoMath.ToRadians(rotation), new Point3d(0, 0, 0));
            Transform transform_minus = Transform.Rotation(RhinoMath.ToRadians(-rotation), new Point3d(0, 0, 0));

            for (int i = 0; i < curves.Count; i++) {
                curves[i].Transform(transform_plus);
            }
            BoundingBox bounding_box = BoundingBox.Empty;
            foreach (var curve in curves) {
                bounding_box.Union(curve.GetBoundingBox(Plane.WorldXY));
            }

            Point3d p1 = bounding_box.Min;
            Point3d p2 = bounding_box.Max;
            p1.Transform(Transform.Translation(-module_size, -module_size, 0));

            Grid grid = new Grid();
            grid.Point0 = p1;
            grid.Xrange = p2.X - p1.X;
            grid.Yrange = p2.Y - p1.Y;
            grid.Xcount = (int)(grid.Xrange / module_size) + 2;
            grid.Ycount = (int)(grid.Yrange / module_size) + 2;
            grid.ModuleSize = module_size;
            grid.DeRotate = transform_minus;
            return grid;

        }
        private static int[,] MakeInteriorMatrix(Grid grid, Curve curve) {
            //0: nothing, 1: partially in, 2: fully inside
            bool[,] vertices = new bool[grid.Xcount, grid.Ycount];
            for (int i = 0; i < grid.Xcount; i++)
            {
                for (int j = 0; j < grid.Ycount; j++)
                {
                    Point3d pnt = new Point3d(i * grid.ModuleSize + grid.Point0.X, j * grid.ModuleSize + grid.Point0.Y, grid.Point0.Z);
                    PointContainment containment = curve.Contains(pnt, Plane.WorldXY, 0);
                    vertices[i, j] = (containment == PointContainment.Inside || containment == PointContainment.Coincident);
                }
            }

            int[,] output = new int[grid.Xcount - 1, grid.Ycount - 1];
            for (int i = 0; i < grid.Xcount - 1; i++)
            {
                for (int j = 0; j < grid.Ycount - 1; j++)
                {
                    output[i, j] = (vertices[i, j] && vertices[i, j + 1] && vertices[i + 1, j] && vertices[i + 1, j + 1]) ? 1 : 0;
                    output[i, j] += (vertices[i, j] || vertices[i, j + 1] || vertices[i + 1, j] || vertices[i + 1, j + 1]) ? 1 : 0;
                }
            }
            return output;
        }
        //constexpr int FREE = 0;
        //constexpr int BLOCKED = -1;
        //constexpr int ACCESS = 1;
        //constexpr int OUTSIDE = 2;
        private static CombinedInteriorMatrix MakeCombinedInteriorMatrix(int[,] interior, int[,] exterior, int[,] access, Grid grid) {
            int[,] output = new int[grid.Xcount - 1, grid.Ycount - 1];
            for (int i = 0; i < grid.Xcount - 1; i++)
            {
                for (int j = 0; j < grid.Ycount - 1; j++)
                {
                    if (interior[i, j] == 2)
                    {
                        // if all grid in the inside, then inside
                        output[i, j] = 0;
                    }
                    else if (access[i, j] > 0)
                    {
                        // fully or partially in the access part
                        output[i, j] = 1;
                    }
                    else if (exterior[i, j] > 0)
                    {
                        output[i, j] = 2;
                    }
                    else output[i, j] = -1;
                }
            }
            CombinedInteriorMatrix cim = new CombinedInteriorMatrix();
            cim.Data = output;
            cim.Width = grid.Xcount - 1;
            cim.Height = grid.Ycount - 1;

            return cim;
        }

        public static List<Rectangle3d> GridsThatContains(Grid grid, CombinedInteriorMatrix bfao_matrix, int element)
        {
            // make a list, make rectangles responding for each matrix place
            Rectangle3d square_base = new Rectangle3d(new Plane(grid.Point0, Vector3d.ZAxis), grid.ModuleSize, grid.ModuleSize);
            List<Rectangle3d> rec_list = new List<Rectangle3d>();
            for (int i = 0; i < bfao_matrix.Width; i++)
            {
                for (int j = 0; j < bfao_matrix.Height; j++)
                {
                    if (bfao_matrix.Data[i, j] == element)
                    {
                        Vector3d move_vector = new Vector3d(i * grid.ModuleSize, j * grid.ModuleSize, 0);
                        //var vecmove = pntij - grid.Point0;
                        Rectangle3d square_new = square_base;
                        square_new.Transform(Transform.Translation(move_vector));
                        square_new.Transform(grid.DeRotate);
                        rec_list.Add(square_new);
                    }
                }
            }
            return rec_list;
        }

        public static CombinedInteriorMatrix BasicGrid(int width, int height)
        {
            CombinedInteriorMatrix output = new CombinedInteriorMatrix();
            output.Width = width;
            output.Height = height;
            output.Data = new int[width, height];
            for(int i = 0; i < width; i++){
                for (int j = 0; j < height; j++){
                    output.Data[i, j] = (i == 0) ? 1 : (i == width - 1) ? 2 : 0;
                }
            }
            return output;
        }

        private static int ValueOf(CombinedInteriorMatrix cim, int i, int j, int max_x, int max_y){
            if (i == -1 || j == -1 || i == max_x || j == max_y) return -1; // on the border
            int center_of_cim_w = cim.Width / 2;
            int center_of_cim_h = cim.Height / 2;
            int center_of_grid_w = max_x / 2;
            int center_of_grid_h = max_y / 2;
            int projected_i = i - center_of_grid_w + center_of_cim_w;
            int projected_j = j - center_of_grid_h + center_of_cim_h;
            if( projected_i < 0 || projected_j < 0  || projected_i >= cim.Width || projected_j >= cim.Height) return -1;
            return cim.Data[projected_i, projected_j];
        }

        public static void SaveCombinedInteriorMatrix(List<CombinedInteriorMatrix> cim, int max_x, int max_y, string Address) {
            using (System.IO.StreamWriter writer = new System.IO.StreamWriter(Address))
            {
                writer.WriteLine($"static_assert(MAX_X <= {max_x},\"MAX_X inconsistency\");");
                writer.WriteLine($"static_assert(MAX_Y <= {max_y},\"MAX_Y inconsistency\");");
                writer.WriteLine($"static_assert(RESET_GRIDS_COUNT <= {cim.Count},\"RESET_GRIDS_COUNT inconsistency\");");

                writer.WriteLine($"int PresetInputArrays[{cim.Count}][{max_x + 2}][{max_y + 2}]={{");
                for (int index = 0; index < cim.Count; index++)
                {
                    writer.WriteLine("\t{");
                    for (int i = -1; i <= max_x ; i++)
                    {
                        writer.Write("\t\t{ ");
                        for (int j = -1; j <= max_y ; j++)
                        {
                            writer.Write($"{ValueOf(cim[index], i, j, max_x, max_y).ToString().PadLeft(2, ' ')}, ");   
                        }
                        writer.WriteLine("},");
                    }
                    writer.WriteLine("},");
                }
                writer.WriteLine("};");
            }
        }

        /*
            Random rnd = new Random();

    // Save the array to a file
    using (System.IO.StreamWriter writer = new System.IO.StreamWriter(file_name))
    {
      writer.Write(String.Format("static_assert(MAX_X == {0},\"MAX_X inconsistency\");\n", max_x));
      writer.Write(String.Format("static_assert(MAX_Y == {0},\"MAX_Y inconsistency\");\n", max_y));
      writer.Write(String.Format("static_assert(RESET_GRIDS_COUNT == {0},\"RESET_GRIDS_COUNT inconsistency\");\n\n", count));

      writer.Write("int PresetInputArrays[RESET_GRIDS_COUNT][MAX_X + 1][MAX_Y + 1]={\n");
      for(int mm = 0; mm < count; mm++){
        int x = rnd.Next(min_x, max_x + 1);
        int y = rnd.Next(min_y, max_y + 1);
        writer.Write("\t{\n");
        for (int i = 0; i < max_x + 1; i++)
        {
          writer.Write("\t\t{");
          for (int j = 0; j < max_y + 1; j++)
          {
            writer.Write((i < x && j < y) ? " 0," : "-1,");
          }
          writer.Write("},\n");
        }
        writer.Write("\t},\n");

      }
      writer.Write("};\n");
    }

         */

    }
}
/*
public class Script_Instance : GH_ScriptInstance{ 
    private void RunScript(Curve Interior, Curve Exterior, Curve Access, double module_size, int count, Point3d PutP0, double Distance, ref object _Interior, ref object _Exterior, ref object _Access, ref object _Blocked)
    {
        if (CheckCurve(Interior) == false || CheckCurve(Exterior) == false || CheckCurve(Interior) == false) return;

        List<Rectangle3d> insides = new List<Rectangle3d>();
        List<Rectangle3d> outsides = new List<Rectangle3d>();
        List<Rectangle3d> accesses = new List<Rectangle3d>();
        List<Rectangle3d> blocked = new List<Rectangle3d>();
        List<CombinedInteriorMatrix> cim = new List<CombinedInteriorMatrix>();
        double step = 360 / count;
        int i = 0;
        for(double rotate = 0; rotate < 360; rotate += step){

            CombinedInteriorMatrix bfao = MakeCombinedInteriorMatrixWithCurvesRotateModuleSize (Interior,  Exterior, Access, module_size,  rotate);
            Grid output_grid = new Grid(new Point3d(PutP0.X, PutP0.Y + Distance * i, 0), module_size);
            insides.AddRange(GridsThatContains(output_grid, bfao, 0));
            outsides.AddRange(GridsThatContains(output_grid, bfao, 2));
            accesses.AddRange(GridsThatContains(output_grid, bfao, 1));
            blocked.AddRange(GridsThatContains(output_grid, bfao, -1));
            cim.Add(bfao);
            i++;
        }


        _Interior = insides;
        _Exterior = outsides;
        _Access = accesses;
        _Blocked = blocked;
    }

}
*/
