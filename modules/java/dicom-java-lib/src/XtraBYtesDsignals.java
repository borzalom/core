package XtraBYtes;

import XtraBYtes.XtraBYtesDICOM;

import org.json.simple.JSONObject;

public abstract class XtraBYtesDsignals  {
   	    	   	   	    	   	    	   	   	
     protected static XtraBYtesDsignals SignalHandler;
	
	  public static XtraBYtesDsignals getSignalHandler() {
	     return SignalHandler;
	  }
	  
	  public void setSignalHandler(XtraBYtesDsignals sh) {
	  	  SignalHandler = sh;	  
	  }
   
     public abstract String ViewPageOverviewContent();   	
   	   	
}