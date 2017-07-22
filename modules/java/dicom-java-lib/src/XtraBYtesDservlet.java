package XtraBYtes;

import XtraBYtes.XtraBYtesDICOM;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.io.IOException;

import javax.servlet.http.*;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;

import org.eclipse.jetty.client.HttpClient;
import org.eclipse.jetty.client.api.ContentResponse;
import org.eclipse.jetty.client.util.StringContentProvider;

import java.net.*; 
import java.net.InetSocketAddress;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.MalformedURLException;


public class XtraBYtesDservlet extends HttpServlet {

    public void doGet(HttpServletRequest request, HttpServletResponse response)
      
      throws ServletException, IOException {
        response.setContentType("text/html");
        PrintWriter ServletOutputStream = response.getWriter();
        ServletOutputStream.println("<h1>" + "The specified method is not allowed." + "</h1>");
    }
    
    public void doPost(HttpServletRequest request, HttpServletResponse response)    
      
      throws ServletException, IOException {    
	        JSONObject JsonRequest = new JSONObject(); 
			  try {
	          JsonRequest = (JSONObject)new JSONParser().parse(request.getReader());
				 
			  }  catch (ParseException e) {  e.printStackTrace();  }
				    		        
	        response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate, private");
	        response.setHeader("Pragma", "no-cache");
	        response.setDateHeader("Expires", 0);
	        response.setContentType("text/plain; charset=UTF-8");
       
           JSONObject JsonResponse = new JSONObject();       
                                 
	        switch ( (String) JsonRequest.get("DICOM") ) {					
					case "Heartbeat-Echo-Request": {    
					  JsonResponse = DICOM_HeartbeatReq(JsonRequest);   
	            } break;
					case "view-page-overview-content": {    
					    JsonResponse = DICOM_ViewPageOverviewContent(JsonRequest);					       
	            } break;
	            	                          	            
	            default: {            	
					   JsonResponse.put("error","Bad requestType.");
	            } break;
	        }                       

           response.setContentType("text");
           PrintWriter ServletOutputStream = response.getWriter();  
      
           ServletOutputStream.print(JsonResponse.toString());
    }
        
	 public synchronized JSONObject SendJsonQuery(JSONObject request) {
	    
	    JSONObject JSONresponse = new JSONObject();	    
	    
	    try {	    	
		    	HttpClient client = new HttpClient();
		    	client.setBindAddress(new InetSocketAddress( InetAddress.getByName("127.0.0.1") , 0 )); 
	         client.start();
		    	ContentResponse response = client.POST("http://127.0.0.1:3344/dicom")
	        .content(new StringContentProvider(request.toString()) , "application/json; charset=UTF-8")
	        .send();                       
	        client.stop(); 
	        if ( response.getStatus() == HttpURLConnection.HTTP_OK ) {
						    try {
						          JSONresponse = (JSONObject)new JSONParser().parse(response.getContentAsString());
						    } catch (Exception e) {
						          JSONresponse.put("Error","Failed parsing returned JSON object.");
						    }		           
		     }             	
	    } catch (Exception e) {
	             JSONresponse.put("Error","Communication error.");
	    }
	        	     
	    return JSONresponse;
	 }    
    
    
    private JSONObject DICOM_HeartbeatReq( JSONObject JsonRequest ) {
    	
       JSONObject response = new JSONObject();
                        
		 try {       
		       response.put("DICOM", "Heartbeat-Echo-Reply");
		       XtraBYtesDICOM.UpdateHeartbeatTimestamp(System.currentTimeMillis());
		 } catch (Exception e) {
				 response.put("error",1);
		 }              
      
       return response;
    }
      
     
    public JSONObject DICOM_ViewPageOverviewContent( JSONObject JsonRequest ) {
    	
       JSONObject response = new JSONObject();       
                        
		 try {       
		       response.put("DICOM", "view-page-overview-content");
		       response.put("view-page-overview-content", XtraBYtesDICOM.getSignalHandler().ViewPageOverviewContent());
		 } catch (Exception e) {
				 response.put("error",1);
		 }              
      
       return response;
    }   
       
}
