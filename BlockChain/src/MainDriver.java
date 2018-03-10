import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class MainDriver {

	public static ArrayList<Block> blockchain = new ArrayList<Block>(); 
	public static int difficulty = 3;
	
	public static void main(String[] args) {
		
		int counter = 0;
		 try{
			 // (1) Hardcoding in the absolute path
			 //FileReader file = new FileReader("/Users/Myhannie/Desktop/UGA/(7) SPRING 2018/CSEE 2920/BlockChain/src/BlockChainData.txt");

			 // (2) Finding the absolute (without Eclipse IDE)
			 //String absolutePath = new File("").getAbsolutePath() + "/BlockChainData.txt";
			 
			 // (3) Finding the absolute (Eclipse IDE)
			 String absolutePath = new File("").getAbsolutePath() + "/src/BlockChainData.txt";

			 // Read the file
			 FileReader file = new FileReader(absolutePath);
			 BufferedReader br = new BufferedReader(file);
			 String line;
			 String data = "\t";
			 while ((line = br.readLine()) != null)   {
				 
				 // Increment counter if not at the end of block
				 if(line.compareTo("---") != 0) {
				 counter++;
				 } // if
				 
				 // Append the lines if not at the end of the block
				 if(line.compareTo("---") != 0) {
					 if(counter % 5 != 0) {
						 data += line + "\n\t\t"; 
					 } // if
					 else{
						 data += line; 
					 } // else
				 } // while
				 
				 // End of the block --> create/add the block
				 else {
					 // Creating the first block
					 if(counter/5 == 1) {
						 blockchain.add(new Block(data, "0"));
					 } // if
					 // Creating non-first blocks
					 else {
						 blockchain.add(new Block(data,blockchain.get(blockchain.size()-1).hash));	 
					 } // else
					 blockchain.get(counter/5 - 1).mineBlock(difficulty);
					 data = "\t";
				 } // else
			 } // while
			 file.close();
		 } // try
		 catch(Exception error){
			 System.err.println("Error: " + error.getMessage());
		 } // catch
		 
		 // Prints the Block Chain
		 System.out.println("\n\t\t\t Block Chain: \n");
		 for(int i = 0; i < blockchain.size(); i++) {
			 System.out.print("Hash:           "); System.out.println(blockchain.get(i).hash);
			 System.out.print("Previous Hash:  "); System.out.println(blockchain.get(i).previousHash);
			 System.out.print("Data:    "); System.out.println(blockchain.get(i).getData());
			 System.out.print("Time Stamp:     "); System.out.println(blockchain.get(i).getTimeStamp());
			 System.out.print("Nonce:          "); System.out.println(blockchain.get(i).getNonce());
			 System.out.println("--------------------------------------------------------------------------------------------");
		 } // for
		
		 // Determine if the Block Chain is valid
		 if(isChainValid() == true){
			 System.out.println("\nThe block chain is valid.");
		 } // if
		 else {
			 System.out.println("\nThe block chain is not valid.");
		 } // else
		
	} // main
	
	
	public static Boolean isChainValid() {
		Block currentBlock; 
		Block previousBlock;
		String hashTarget = new String(new char[difficulty]).replace('\0', '0');
		
		//loop through blockchain to check hashes:
		for(int i=1; i < blockchain.size(); i++) {
			currentBlock = blockchain.get(i);
			previousBlock = blockchain.get(i-1);
			
			//compare registered hash and calculated hash:
			if(!currentBlock.hash.equals(currentBlock.calculateHash()) ){
				System.out.println("Current Hashes not equal");			
				return false;
			} // if
			
			//compare previous hash and registered previous hash
			if(!previousBlock.hash.equals(currentBlock.previousHash) ) {
				System.out.println("Previous Hashes not equal");
				return false;
			} // if
			
			//check if hash is solved
			if(!currentBlock.hash.substring( 0, difficulty).equals(hashTarget)) {
				System.out.println("This block hasn't been mined");
				return false;
			} // if
			
		} // for
		
		return true;
	} // isChainValid
	
} // MainDriver

