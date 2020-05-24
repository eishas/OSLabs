using System;
using System.Collections.Generic;

namespace OsLab3
{
    class Program
    {
    static void Main(string[] args)
    {
      Console.WriteLine("Lab 3");
      for (int intensity = 30; intensity > 0; intensity--)
      {
        int delay = intensity;
        var gen = GenFactory.getGenerator(3, 10, 0, 10);
        var rnd = new Random();

        var pq = new PriorityQueue(30);
        for (int i = 0; i < 10000; ++i)
        {
          if (--delay == 0)
          {
            gen.MoveNext();
            pq.addPackageToTheQueu(gen.Current);
            delay = intensity;
          }
          pq.tick();
        }
        int tavrg = pq.aveageTime / pq.packageNumber;

        Console.WriteLine("Середнiй час очiкування вiд iнтенсивностi:");
        Console.WriteLine("Iнтенсивнiсть: " + intensity + " час очiкування: " + tavrg);
        Console.WriteLine("Середнiй час простою вiд iнтенсивностi:");
        Console.WriteLine("Iнтенсивнiсть: " + intensity + " час простою: "+pq.dt/100);
      }
      {
    }
  }

  class DataPackage
  {
    public DataPackage(int t, int p)
    {
      Time = t;
      Priority = p;
    }
    public int Time { get; set; }
    public int Priority { get; set; }

  }

  class PriorityQueue
  {
    public int size;
    public int aveageTime = 0;
    public int dt = 0;
    public int packageNumber = 0;
    List<DataPackage> pq = new List<DataPackage>();
    public PriorityQueue(int size)
    {
      this.size = size;
    }
    public void tick()
    {
      aveageTime += pq.Count;
      if (pq.Count == 0)
      {
        dt++;
      }
      else
      {
        pq[0].Time--;
        if (pq[0].Time == 0)
        {
          pq.RemoveAt(0);
        }
      }
    }

    public void addPackageToTheQueu(DataPackage pc)
    {
      if (pq.Count < size)
      {
        packageNumber++;
        int idx = 0;
        for (; idx < pq.Count; idx++)
        {
          if (pq[idx].Priority > pc.Priority)
          {
            break;
          }
        }
        pq.Insert(idx, pc);
      }
    }
  }


  class GenFactory
  {
    public static IEnumerator<DataPackage> getGenerator(int minTime, int maxTime,
        int minPriority, int maxPriority)
    {
      Random rnd = new Random();
      while (true)
      {
        yield return new DataPackage(rnd.Next(minTime, maxTime),
            rnd.Next(minPriority, maxPriority));
      }
    }
  }
}






}
