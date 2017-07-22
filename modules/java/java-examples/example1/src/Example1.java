package XtraBYtes;

import XtraBYtes.XtraBYtesDsignals;
import XtraBYtes.XtraBYtesDICOM;

public class Example1  {
    
   public static class XBYsignals extends XtraBYtesDsignals {	
          
       public String ViewPageOverviewContent() {
          return "Hello XtraBYtes ;-)";       
       }
       
   }
	
   public static void main(String[] args) {
     
     XtraBYtesDICOM module = new XtraBYtesDICOM();
     module.setSignalHandler((XtraBYtesDsignals)new XBYsignals());     
     module.startDICOM(
            "Example1 Module", // Module name
            "example1", 		 // Module ID
            10001, 				 // Module DICOM port
            new String[]{  	 // Signals
            	 "view-page-overview-content"  
            }	       
        );         
  }
	
}
