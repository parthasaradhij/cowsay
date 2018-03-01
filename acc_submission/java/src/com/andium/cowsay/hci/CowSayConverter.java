package com.andium.cowsay.hci;

import java.io.File;
import java.util.Scanner;

public class CowSayConverter 
{
	private static final int MAXCHARLEN = 80;

	private static final int MAXROWSIZE = MAXCHARLEN/2;
	
	static
	{
		try
		{
			
			System.loadLibrary("cowsay");
		}
		catch(Exception e)
		{
			System.err.println("Unable to load library: cowsay");
		}
	}
	public Process cowProcess;
	
	public native void hci_init();
	private native String[] convert_to_cowsay(String input);
	public static void main(String[] args)
	{

		//testCowSayConverter();
		
		
		CowSayConverter converter = new CowSayConverter();
		converter.initCowSayConverter();
	
			
		while(true)
		{
			System.out.println("Enter text (max 80 characters) to get it converted to cowsay");

			Scanner scan = new Scanner(System.in);
			String input = null;
			try
			{
				input = scan.nextLine();
			}
			catch(Exception e)
			{
				System.err.println("Error while reading, kill the cow process explicitly!");
			}
			
			converter.convertAndValidate(input);
			
		}
	}
	
	public static void testCowSayConverter()
	{
		CowSayConverter converter = new CowSayConverter();
		converter.initCowSayConverter();

		try
		{
		
		// testcase 1: Null string
		converter.convertAndValidate(null);

		//testcase 2: String of length less than 80 chars
		converter.convertAndValidate("Hello world");
	
		//testcase 2: String of length 80 chars
		converter.convertAndValidate("Hello world, this is a program which tests concepts like JNI, IPCs, dynamic libs");
		
		//testcase 2: String of length more than 80 chars
		converter.convertAndValidate("Hello world, this is a program which tests concepts like JNI, IPCs, dynamic libs, etc");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public void convertAndValidate(String input)
	{
		if(null == input)
		{
			System.err.println("Enter text of length not more than 80 characters");
			return;
		}
		if(input.length() > MAXCHARLEN)
		{
			System.err.println("Enter text of length not more than 80 characters");
		}
		else
		{
			System.out.println("Cowsay converted text:");
			//System.out.println(input);
			String[] output = convert_to_cowsay(input);
			for (String s: output) 
			{           
				System.out.print(s); 
			}
			if(validateOutput(input, output))
			{
				System.out.println("Correct ouput");
			}
			else
			{
				System.err.println("Incorrect output");
			}
		}
		
	}

	public void initCowSayConverter()
	{
		// Creates and initializes the shared memory
		HCI_initializer hciInitializer = new HCI_initializer(this);
		
		hciInitializer.start();
		
	
		// Launch the 'cow say' converter
		launchCowSayConverter();
		
	}
	
	
	private boolean validateOutput(String input, String[] output) 
	{
		String emptyRow = "                                              ";
		char[] rowArr = emptyRow.toCharArray();
		
		String row0 = "  ________________________________________  \n";
		String row1 = "";
		String row2 = "";
		String row3 = "  ----------------------------------------  \n";
		String row4 = "       \\   ^__^                         \n";
		String row5 = "        \\  (oo)\\_______                 \n";
		String row6 = "           (__)\\       )\\/\\             \n";
		String row7 = "               ||----w |                \n";
		String row8 = "               ||     ||                \n";		
		

		int len = input.length();
		
		if(len > MAXROWSIZE)
		{
			row1 = "/ ";
			row1 = row1 + input.substring(0,MAXROWSIZE) + " \\\n";
			
			row2 = "\\ ";

			char[] tmp = input.substring(MAXROWSIZE, len).toCharArray();
			
			char[] rowArr1 = emptyRow.substring(2,emptyRow.length()-4).toCharArray();


			
			System.arraycopy(tmp, 0, rowArr1, 0, len-MAXROWSIZE);
			row2 = row2 + new String(rowArr1) + " /\n";
		}
		else
		{
			row1 = "/ ";

			char[] tmp = input.toCharArray();
			
			char[] rowArr1 = emptyRow.substring(2,emptyRow.length()-4).toCharArray();
			
			System.arraycopy(tmp, 0, rowArr1, 0, len);
			row1 = row1 + new String(rowArr1) + " \\\n";
		
					
			row2 = "\\ " + emptyRow.substring(2, emptyRow.length()-4) + " /\n";
		}	
		boolean valid = true;

		/*
		System.out.print(row0);
		System.out.print(row1);
		System.out.print(row2);
		System.out.print(row3);
		System.out.print(row4);
		System.out.print(row5);
		System.out.print(row6);
		System.out.print(row7);
		System.out.print(row8);
		*/
		
		if(!row0.equals(output[0]))
		{
			System.out.println("ROW 0 not matching");
		
			valid = false;
		}
		else if(!row1.equals(output[1]))
		{
			System.out.println("ROW 1 not matching");
			System.out.print(output[1]);
			System.out.print(row1);
			valid = false;
		}
		else if(!row2.equals(output[2]))
		{
			System.out.println("ROW 2 not matching");
			valid = false;
		}		
		else if(!row3.equals(output[3]))
		{
			System.out.println("ROW 3 not matching");
			valid = false;
		}		
		else if(!row4.equals(output[4]))
		{
			System.out.println("ROW 4 not matching");
			valid = false;
		}		
		else if(!row5.equals(output[5]))
		{
			System.out.println("ROW 5 not matching");
			valid = false;
		}		
		else if(!row6.equals(output[6]))
		{
			System.out.println("ROW 6 not matching");
			valid = false;
		}		
		else if(!row7.equals(output[7]))
		{
			System.out.println("ROW 7 not matching");
			valid = false;
		}		
		else if(!row8.equals(output[8]))
		{
			System.out.println("ROW 8 not matching");
			valid = false;
		}
		
		return valid;
		
	}
	/**
           This launches the 'cow' process, that converts the human readable text into 'cow
           launguage as specified in the requirement.
	*/
	public void launchCowSayConverter()
	{
		// Some quick work-around to go to the relative path ....
		File currentDir = new File("");
		System.out.println(currentDir.getAbsolutePath());
		File cowPath = new File(currentDir.getAbsoluteFile() +"/bin/cow");
		System.out.println("PATH " + cowPath.getAbsolutePath());
				
		try
		{
			ProcessBuilder pb = new ProcessBuilder(cowPath.getAbsolutePath());

			cowProcess = pb.start();
		}
		catch(Exception e)
		{
			System.err.println("Can't launch cow, exiting...");
			e.printStackTrace();
			System.exit(1);
		}
		
	}

}
class HCI_initializer extends Thread
{
	CowSayConverter converter = null;

	public HCI_initializer(CowSayConverter converter)
	{
		this.converter = converter;
	}
	public void run()
	{
		converter.hci_init();
		
	}
}


