using System;

class MainClass
{
 	public static void Main (string[] args)
	{
		CowConfig.WriteConfig writeCfg = new CowConfig.WriteConfig("test.txt");
		writeCfg.Section("Ints");
		writeCfg.WriteLine("three: ", 3.ToString());
		writeCfg.Section("Strings");
		writeCfg.WriteLine("ell: ", "L");
		writeCfg.WriteLine("tee: ", "T");
		writeCfg.Close();

		CowConfig.ReadConfig readCfg = new CowConfig.ReadConfig("test.txt");
		Console.WriteLine(readCfg.Read<string>("Strings", "ell: "));
		Console.WriteLine(readCfg.Read<int>("Ints", "three: "));
		Console.WriteLine(readCfg.Read<int>("Ints", "three: "));
		Console.WriteLine(readCfg.Read<string>("Strings", "tee: "));
	}
}
