//Source: https://medium.com/programmers-blockchain/create-simple-blockchain-java-tutorial-from-scratch-6eeed3cb03fa

import java.security.MessageDigest;

public class StringUtil {
	
	//Applies Sha256 to a string and returns the result. 
	public static String applySha256(String input){		
		
		try {
			MessageDigest digest = MessageDigest.getInstance("SHA-256");	        
			//Applies sha256 to our input, 
			byte[] hash = digest.digest(input.getBytes("UTF-8"));	        
			StringBuffer hexString = new StringBuffer(); // This will contain hash as hexidecimal
			for (int i = 0; i < hash.length; i++) {
				String hex = Integer.toHexString(0xff & hash[i]);
				if(hex.length() == 1) hexString.append('0');
				hexString.append(hex);
			} // for
			return hexString.toString();
		} // try
		
		catch(Exception e) {
			throw new RuntimeException(e);
		} // catch
		
	} // applySha256
	
} // StringUtil

