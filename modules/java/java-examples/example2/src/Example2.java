package XtraBYtes;

import XtraBYtes.XtraBYtesDsignals;
import XtraBYtes.XtraBYtesDICOM;

import java.util.Calendar;

public class Example2  {
    
   public static class XBYsignals extends XtraBYtesDsignals {	
          
       public String ViewPageOverviewContent() {
       	
          Calendar cal = Calendar.getInstance();
          int hour = cal.get( Calendar.HOUR_OF_DAY );
          
          if( hour > 5 && hour < 12 ) return "Good morning XBY!";            
          if( hour > 11 && hour < 17 ) return "Good afternoon XBY!";      
          if( hour > 17 && hour < 19 ) return "Good evening XBY!";
          return "Good night XBY!";
                 
       }       
   }
	
   public static void main(String[] args) {
     
     XtraBYtesDICOM module = new XtraBYtesDICOM();
     module.setSignalHandler((XtraBYtesDsignals)new XBYsignals());     
     module.startDICOM(
            "Example2 Module", // Module name
            "example2", 		 // Module ID
            10002, 				 // Module DICOM port
            new String[]{  	 // Signals
            	 "view-page-overview-content"  
            }	       
        );         
  }
	
}
