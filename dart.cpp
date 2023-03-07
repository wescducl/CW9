using System;
using System.Collections.Generic;
using System.Threading;

class FindPiThread
{
    private int dartsToThrow;
    private int dartsInside;
    private Random random;

    public FindPiThread(int dartsToThrow)
    {
        this.dartsToThrow = dartsToThrow;
        this.dartsInside = 0;
        this.random = new Random();
    }

    public int GetDartsInside()
    {
        return dartsInside;
    }

    public void throwDarts()
    {
        for (int i = 0; i < dartsToThrow; i++)
        {
            double x = random.NextDouble();
            double y = random.NextDouble();
            if (x * x + y * y <= 1)
            {
                Interlocked.Increment(ref dartsInside);
            }
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        Console.Write("Enter the number of throws per thread: ");
        int dartsPerThread = int.Parse(Console.ReadLine());

        Console.Write("Enter the number of threads: ");
        int numThreads = int.Parse(Console.ReadLine());

        List<Thread> threadList = new List<Thread>();
        List<FindPiThread> piThreadList = new List<FindPiThread>();

        // create threads and pi thread objects
        for (int i = 0; i < numThreads; i++)
        {
            FindPiThread piThread = new FindPiThread(dartsPerThread);
            Thread thread = new Thread(new ThreadStart(piThread.throwDarts));
            threadList.Add(thread);
            piThreadList.Add(piThread);

            thread.Start();
            Thread.Sleep(16); // pause to ensure unique seed
        }

        // wait for all threads to finish
        foreach (Thread thread in threadList)
        {
            thread.Join();
        }

        // collect results from each pi thread object
        int totalDartsInside = 0;
        foreach (FindPiThread piThread in piThreadList)
        {
            totalDartsInside += piThread.GetDartsInside();
        }

        // calculate pi estimation
        double piEstimation = 4.0 * totalDartsInside / (dartsPerThread * numThreads);

        // print results
        Console.WriteLine($"Estimation of pi: {piEstimation:F8}");
    }
}
