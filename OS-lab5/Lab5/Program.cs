using System;

namespace Lab5
{
    class Program
    {
        static void Main(string[] args)
        {
            var start = DateTime.Now;
            int[] Vector = new int[2];

            for (int j = 490000000; j > 0; j--)
            {
                Vector[0] = Vector[0] + 2;
            }

            Vector[1] = Vector[0];
            Console.WriteLine(Vector[0]);

            var finish = DateTime.Now;
            Console.WriteLine($"Час виконання программи: {finish - start}");
        }
    }
}
