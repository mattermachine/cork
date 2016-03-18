using System;
using System.Runtime.InteropServices;

namespace UnityCork
{
    public class CorkGateway
    {
        //=== sending the data
        [DllImport ( "cork" )]
        private static extern bool CreateTriMesh ( float[] vertices, uint n_vertices, uint[] faces, uint n_faces, int target );

        //=== doing the Ops
        [DllImport ( "cork")]
        private static extern bool ComputeUnion ( );

        [DllImport ( "cork" )]
        private static extern bool ComputeIntersection ();

        [DllImport ( "cork" )]
        private static extern bool ComputeDifference ();

        [DllImport ( "cork" )]
        private static extern bool ComputeSymmetricDifference ();

        //=== getting data out of the current mesh
        [DllImport ( "cork" )]
        private static extern uint GetNbVertices ( );

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


        public static float[] GetVerticesList ()
        {
            float[] result = null;
            uint size;

            IntPtr arrayValue = IntPtr.Zero;
            try
            {
                arrayValue = GetVertices ( out size );
                if ( arrayValue != IntPtr.Zero )
                {
                    result = new float[size];
                    Marshal.Copy ( arrayValue, result, 0, Convert.ToInt32 ( size ) );
                }
            }
            finally
            {
                FreeFloatList ( arrayValue );
            }

            return result;
        }

        public static int[] GetFaceList ()
        {
            int[] result = null;
            uint size;

            IntPtr arrayValue = IntPtr.Zero;
            try
            {
                arrayValue = GetFaces ( out size );
                if ( arrayValue != IntPtr.Zero )
                {
                    result = new int[size];
                    Marshal.Copy ( arrayValue, result, 0, Convert.ToInt32(size) );
                }
            }
            finally
            {
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

        public static bool ExecuteBooleanOp ( string type )
        {
            switch ( type )
            {
                case "union":
                    ComputeUnion ();
                    break;
                case "difference":
                    ComputeDifference ();
                    break;
                case "intersection":
                    ComputeIntersection ();
                    break;
                case "symmetric_difference":
                    ComputeSymmetricDifference ();
                    break;
            }
            return true;
        }

        public static int TestSetup ()
        {
            return 42;
        }
    }
}
