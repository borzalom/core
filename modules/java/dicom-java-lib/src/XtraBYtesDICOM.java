package XtraBYtes;

import XtraBYtes.XtraBYtesDservlet;
import XtraBYtes.XtraBYtesDsignals;

import java.io.IOException;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
 
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.handler.AbstractHandler;
import org.eclipse.jetty.server.ServerConnector;
import org.eclipse.jetty.server.handler.ResourceHandler;
import org.eclipse.jetty.server.handler.DefaultHandler;
import org.eclipse.jetty.server.handler.HandlerList;
import org.eclipse.jetty.servlet.ServletHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.eclipse.jetty.servlet.ServletContextHandler;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class XtraBYtesDICOM extends XtraBYtesDsignals {
	
		public String ViewPageOverviewContent() { return ""; }	
	
	   private static long HeartbeatTimestamp = 0;

      public static void UpdateHeartbeatTimestamp(long timestamp) {
         HeartbeatTimestamp = timestamp;
      }
        	
		private String AcceptedMSG = "Module accepted";	 
		private String AcceptedMSG2 = "STOP FLOOD!";   	
	
	   private JSONObject ModuleConnectionRequest = new JSONObject();
	
      private Server DICOMserver;	
	
	   // 0 = Invalid  1 = Connected  2 = Disconnected
	   private int ModuleStatus=0;

      private boolean ModuleWatchdogStarted = false;

      private ScheduledExecutorService CronThreads = Executors.newScheduledThreadPool(1);

      private final Runnable ModuleWatchdog = new Runnable() {
         public void run() { 
           if (( ModuleStatus == 1 ) && ((System.currentTimeMillis()-HeartbeatTimestamp)>2000)) {
         	  ModuleStatus = 2;
         	  System.out.print("Connection lost. Reconnecting.");            
              reconnect();            
           }    
         }
      };    

      public boolean connect(String ModuleName, String ModuleID, int ModulePort, String[] Signals){
        
	      if (!ModuleWatchdogStarted) {
	        CronThreads.scheduleWithFixedDelay(ModuleWatchdog, 0, 1, TimeUnit.SECONDS);	
	        ModuleWatchdogStarted = true;
	      }      

	    	ModuleConnectionRequest.put("DICOM","Module-Registration");
	    	ModuleConnectionRequest.put("module-name",ModuleName);
	    	ModuleConnectionRequest.put("module-id",ModuleID);
	    	ModuleConnectionRequest.put("dicom-port",ModulePort);
	    	JSONArray signal_wrapper = new JSONArray();
	    	for (String signal: Signals) {  
	    	   signal_wrapper.add(signal);
	    	}    	
	    	ModuleConnectionRequest.put("signals",signal_wrapper);
             
	      while (true) {	      	
		      XtraBYtesDservlet listenner = new XtraBYtesDservlet();  
		    	JSONObject response = listenner.SendJsonQuery(ModuleConnectionRequest);    	
		      if ((AcceptedMSG.equals((String)response.get("DICOM"))) || (AcceptedMSG2.equals((String)response.get("DICOM")))) {
		      	System.out.println("OK");	            
	        		DICOMserver = new Server();
	        		ServerConnector connector;    
	        		connector = new ServerConnector(DICOMserver);    
	    
	            connector.setPort(ModulePort);
	            connector.setHost("127.0.0.1");
	            connector.setIdleTimeout(15000);
	            connector.setReuseAddress(true);
	            DICOMserver.addConnector(connector);
	                
		      	HandlerList DICOMHandlers = new HandlerList();
		      	
		      	ServletContextHandler DICOMHandler = new ServletContextHandler();    
	                                  
	            DICOMHandler.addServlet(XtraBYtesDservlet.class, "/dicom");
	
	            DICOMHandlers.addHandler(DICOMHandler);
	
	            DICOMHandlers.addHandler(new DefaultHandler());
	
	            DICOMserver.setHandler(DICOMHandlers);
	            DICOMserver.setStopAtShutdown(true);	
	            
	            try {
	                  HeartbeatTimestamp = System.currentTimeMillis();    
	                  DICOMserver.start();
	                  System.out.println(ModuleName + " DICOM server started at " + ModulePort + " port.");
	                  ModuleStatus = 1;
	                  return true;
	            } catch (Exception e) {
	                  System.out.println("Failed to start DICOM server.");
	                  return false;
	            }
		         
		     } // if
		     try {
	            Thread.sleep(1000);
	        } catch (InterruptedException ex) { }
		     System.out.print("."); 
	      } 
	    }
	
      private void reconnect() {
	    	 while (true) {
			      XtraBYtesDservlet listenner = new XtraBYtesDservlet();  	    	
			    	JSONObject response = listenner.SendJsonQuery(ModuleConnectionRequest);    	
			      if ((AcceptedMSG.equals((String)response.get("DICOM"))) || (AcceptedMSG2.equals((String)response.get("DICOM")))) {
			      	System.out.println("Reconnected.");
			      	ModuleStatus = 1;
			      	HeartbeatTimestamp = System.currentTimeMillis();
			      	return;
		         }
			      try {
		            Thread.sleep(1000);
		         } catch (InterruptedException ex) { }
		    	   System.out.print(".");
	    	 }
      }	
        
      public void startDICOM(String ModuleName, String ModuleID, int ModulePort, String[] Signals){
    	
            Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
                 @Override
                 public void run() {
                    shutdown();
                 }
            }));
        
        		try {
			          System.out.println("*** Starting XtraBYtes module.");
			          if (!connect( ModuleName, ModuleID, ModulePort, Signals )) throw new Exception("Module registration failed.");                                    
                   System.out.println("*** XtraBYtes module started successfully.");
		      } catch (Exception e) {
		             System.out.println("*** ERROR: "+e.toString());
		             System.out.println("*** XtraBYtes module start FAILED.");
		      }     	

    }
	
    public void shutdown() {        
    	  CronThreads.shutdown();   
        System.out.println("*** XtraBYtes module stopped.");     
    }
	
}