using System;
using System.Drawing;
using System.Runtime.Remoting.Messaging;

namespace LAB2
{

    
    internal static class Program
    {
        private static readonly Color color = Color.Black;
        
        public static void Main(string[] args)
        {
            var arguments = ArgumentProvider.ReadAndValidateArguments();
            var bitmap = new Bitmap(arguments.Width,arguments.Height);

            Func<float, int> evaluatedFunc = x => (int)(arguments.A * x * x + arguments.B * x + arguments.C);

            var x1 = (int)(-arguments.B / (2 * arguments.A));
            var anchorPoint  = new Point(x1,evaluatedFunc(x1));



            var cord1 = new Point(anchorPoint.X,evaluatedFunc(anchorPoint.X));
            for(int currentX = anchorPoint.X; currentX < arguments.Width ; currentX++) //алгоритм брезенхема
            {
                var cord2 = new Point(currentX + 1,evaluatedFunc(currentX + 1));

                float dx = 1;
                float dy = Math.Abs(cord2.Y - cord1.Y);
                int signY = Math.Sign(cord2.Y - cord1.Y);
                   
                float derr = 1 - dy;
                
                while (cord1.X != cord2.X || cord1.Y != cord2.Y)
                {
                    float error = derr * 2;
                    if (derr > - dy)
                    {
                        derr -= dy;
                        cord1.X += 1;     
                    }
                    if (error < dx)
                    {
                        derr += dx;
                        cord1.Y += signY;
                    }
                    DrawPixel(bitmap,anchorPoint,cord1);
                } 
            }

            var anchor = ToScreenCords(bitmap,anchorPoint);
            if(ValidatePoint(bitmap,anchor))
                bitmap.SetPixel((int)anchor.X,(int)anchor.Y,color);
            bitmap.Save($"{arguments.FileName}.bmp");
        }

        private static void DrawPixel(Bitmap bmp, Point anchorPoint, Point drawPoint)
        {
            var point = ToScreenCords(bmp, drawPoint);
            if(ValidatePoint(bmp,point))
                bmp.SetPixel((int)point.X,(int)point.Y,color); // рисуем точку
            var point2 = ToScreenCords(bmp, new Point(anchorPoint.X - Math.Abs(anchorPoint.X - drawPoint.X), drawPoint.Y));
            if(ValidatePoint(bmp,point2))
                bmp.SetPixel((int)point2.X,(int)point2.Y,color); // рисуем точку не второй ветви
        }

        private static Point ToScreenCords(Bitmap bmp, Point drawPoint)
        {
            return new Point(drawPoint.X + bmp.Width / 2,bmp.Height / 2 - drawPoint.Y);
        }

        private static bool ValidatePoint(Bitmap bmp, Point drawPoint)
        {
            return 0 < drawPoint.X && drawPoint.X < bmp.Width
                                    && drawPoint.Y < bmp.Height && 0 < drawPoint.Y;
        }
    }
}