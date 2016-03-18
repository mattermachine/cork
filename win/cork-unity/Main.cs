using System;
using System.Runtime.InteropServices;

namespace UnityCork
{
    public class CorkGateway
    {
        //=== sending the data
        [DllImport ( "dllcork2" )]
        private static extern bool CreateTriMesh ( float[] vertices, uint n_vertices, uint[] faces, uint n_faces, int target );

        //=== doing the Ops
        [DllImport ( "dllcork2")]
        private static extern bool ComputeUnion ( );

        [DllImport ( "dllcork2" )]
        private static extern bool ComputeIntersection ();

        [DllImport ( "dllcork2" )]
        private static extern bool ComputeDifference ();

        [DllImport ( "dllcork2" )]
        private static extern bool ComputeSymmetricDifference ();

        //=== getting data out of the current mesh
        [DllImport ( "dllcork2" )]
        private static extern uint GetNbVertices ( );

        [DllImport ( "dllcork2" )]
        private static extern uint GetNbFaces ();

        [DllImport ( "dllcork2" )]
        private static extern IntPtr GetFaces ( out uint size );

        [DllImport ( "dllcork2" )]
        private static extern IntPtr GetVertices ( out uint size );


        [DllImport ( "dllcork2" )]
        private static extern void RemoveIntList ( IntPtr array );

        [DllImport ( "dllcork2" )]
        private static extern void RemoveFloatList ( IntPtr array );

        [DllImport ( "dllcork2" )]
        private static extern void MoveResultToMesh1 ();

        [DllImport ( "dllcork2" )]
        private static extern void EndOp ();


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
                // don't forget to free the list
                RemoveFloatList ( arrayValue );
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
                // don't forget to free the list
                RemoveIntList ( arrayValue );
            }

            return result;
        }

        public static void SendMesh ( float[] vertices, uint n_vertices, uint[] faces, uint n_faces, int target )
        {
            CreateTriMesh( vertices, n_vertices, faces, n_faces, target );
        }

        public static void MoveResultToMesh ()
        {
            MoveResultToMesh1 ();
        }

        public static void CleanState ()
        {
            EndOp ();
        }

        public static bool ExecuteBooleanOp ( string op_type )
        {
            switch ( op_type )
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
                case "symetric_difference":
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


