using System;
using System.Globalization;

namespace LAB2
{
    public static class ArgumentProvider
    {
        public static Arguments ReadAndValidateArguments()
        {
            Arguments args = new Arguments();
            Console.WriteLine("Enter the output resolution:");
            
            Console.Write("Width:");
            while(!int.TryParse(Console.ReadLine(), out args.Width) || args.Width < 100)
            {
                Console.WriteLine("Width must be positive integer value greater then 100.");
                Console.Write("Width:");
            }
            Console.Write("Height:");
            while(!int.TryParse(Console.ReadLine(), out args.Height) || args.Height < 100)
            {
                Console.WriteLine("Height must be positive integer value greater then 100.");
                Console.Write("Height:");
            }
            
            Console.WriteLine("Enter argument for function Ax^2 + Bx + C");
            Console.Write("A:");
            while(!float.TryParse(Console.ReadLine(), out args.A))
            {
                Console.WriteLine("A must be float value with ',' delimiter");
                Console.Write("A:");
            }
            
            Console.Write("B:");
            while(!float.TryParse(Console.ReadLine(), out args.B))
            {
                Console.WriteLine("B must be float value with ',' delimiter");
                Console.Write("B:");
            }
            
            Console.Write("C:");
            while(!float.TryParse(Console.ReadLine(), out args.C))
            {
                Console.WriteLine("C must be float value with ',' delimiter");
                Console.Write("C:");
            }
            Console.Write("Enter filename to save without file type:");
            args.FileName = Console.ReadLine();
            
            Console.WriteLine();

            return args;
        }
        
    }

    public class Arguments
    {
        public int Width = 0;
        public int Height = 0;
        public float A = 0;
        public float B = 0;
        public float C = 0;
        public string FileName = "result";
    }
}