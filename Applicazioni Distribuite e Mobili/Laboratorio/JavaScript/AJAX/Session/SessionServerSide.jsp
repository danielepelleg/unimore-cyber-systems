<%@ page import="java.util.*"%><% response.setContentType("text/xml");%><?xml version="1.0" ?>
<message>
   <sessionID><%=session.getId()%></sessionID>
   <creationDate><%=(new Date(session.getCreationTime()))%></creationDate>
   <accessDate><%=(new Date(session.getLastAccessedTime()))%></accessDate>
<%! String Operation;%>
<%! int c;%>
<%
		Operation = request.getParameter("go");
		if (session.getAttribute("Counter") != null){
			c=(int) session.getAttribute("Counter");
		} else {
			c=0;
		}
		if ("add".equals(Operation)){c++;}
		if ("remove".equals(Operation)){c--;}
		session.setAttribute("Counter", c);
%>
   <counter><%=c%></counter>
</message>

