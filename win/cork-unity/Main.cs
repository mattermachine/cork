using System;
using System.Runtime.InteropServices;

namespace UnityCork
{
    public class Cork
    {
        //=== sending the data
        [DllImport ( "cork" )]
        private static extern bool CreateTriMesh ( float[] vertices, uint n_vertices, uint[] faces, uint n_faces, int target );

        //=== doing the Ops
        [DllImport ( "cork" )]
        private static extern bool ComputeUnion ( bool doSolidCheck );

        [DllImport ( "cork" )]
        private static extern bool ComputeIntersection ( bool doSolidCheck );

        [DllImport ( "cork" )]
        private static extern bool ComputeDifference ( bool doSolidCheck );

        [DllImport ( "cork" )]
        private static extern bool ComputeSymmetricDifference ( bool doSolidCheck );

        //=== getting data out of the current mesh
        [DllImport ( "cork" )]
        private static extern uint GetNbVertices ();

        [DllImport ( "cork" )]
        private static extern uint GetNbFaces ();

        [DllImport ( "cork" )]
        private static extern IntPtr GetFaces ( out uint size );

        [DllImport ( "cork" )]
        private static extern IntPtr GetVertices ( out uint size );


        [DllImport ( "cork" )]
        private static extern void FreeIntList ( IntPtr array );

        [DllImport ( "cork" )]
        private static extern void FreeFloatList ( IntPtr array );

        [DllImport ( "cork" )]
        private static extern void RecycleResult ();

        [DllImport ( "cork" )]
        private static extern void ResetMeshes ();

        [DllImport ( "cork" )]
        private static extern IntPtr GetErrorMessage ();
        

        public static float[] GetVerticesList ()
        {
            float[] result = null;
            uint size;

            IntPtr arrayValue = IntPtr.Zero;
            try {
                arrayValue = GetVertices ( out size );
                if ( arrayValue != IntPtr.Zero ) {
                    result = new float[size];
                    Marshal.Copy ( arrayValue, result, 0, Convert.ToInt32 ( size ) );
                }
            }
            finally {
                FreeFloatList ( arrayValue );
            }

            return result;
        }

        public static int[] GetFaceList ()
        {
            int[] result = null;
            uint size;

            IntPtr arrayValue = IntPtr.Zero;
            try {
                arrayValue = GetFaces ( out size );
                if ( arrayValue != IntPtr.Zero ) {
                    result = new int[size];
                    Marshal.Copy ( arrayValue, result, 0, Convert.ToInt32 ( size ) );
                }
            }
            finally {
                FreeIntList ( arrayValue );
            }

            return result;
        }

        public static void SendMesh ( float[] vertices, uint n_vertices, uint[] faces, uint n_faces, int target )
        {
            CreateTriMesh ( vertices, n_vertices, faces, n_faces, target );
        }

        public static void RecycleMesh ()
        {
            RecycleResult ();
        }

        public static void Reset ()
        {
            ResetMeshes ();
        }

        public static string GetCorkError ()
        {
            return Marshal.PtrToStringAnsi ( GetErrorMessage () );
        }

        public static bool ExecuteBooleanOp ( string type, bool doSolidCheck )
        {
            bool success = true;

            switch ( type ) {
                case "union":
                    success = ComputeUnion ( doSolidCheck );
                    break;
                case "difference":
                    success = ComputeDifference ( doSolidCheck );
                    break;
                case "intersection":
                    success = ComputeIntersection ( doSolidCheck );
                    break;
                case "symmetric_difference":
                    success = ComputeSymmetricDifference ( doSolidCheck );
                    break;
            }

            return success;
        }

        public static int TestSetup ()
        {
            return 42;
        }
    }
}