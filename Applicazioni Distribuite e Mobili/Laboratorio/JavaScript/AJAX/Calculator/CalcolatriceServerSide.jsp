<%@ page import="java.io.*"%><% response.setContentType("text/xml");%><?xml version="1.0" ?>
<%!    
    protected String getResult(String a, String b, String op){
    	String res="NA";
    	if (a==null){return res;}
    	if (b==null){return res;}
    	if (op==null){return res;}
    	float fa=Float.valueOf(a);
    	float fb=Float.valueOf(b);
    	if ("somma".equals(op)){res=""+(fa+fb);}
    	if ("sottrazione".equals(op)){res=""+(fa-fb);}
    	if ("moltiplicazione".equals(op)){res=""+(fa*fb);}
    	if ("divisione".equals(op)){
    		if (fb==0){
    			res="division by 0";	
    		} else {
    			res=""+(fa/fb);
    		}
    	}
    	return res;
    }
%>    
<%		
	String url = request.getRequestURL().toString();
	String a=request.getParameter("a");
	String b=request.getParameter("b");
	String op=request.getParameter("operazione");
 	String res=getResult(a, b, op);
%>
<message>
   <res><%= res%></res>
</message>
	