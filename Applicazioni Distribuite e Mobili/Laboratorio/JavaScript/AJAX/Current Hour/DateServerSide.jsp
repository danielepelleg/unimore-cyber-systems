<%@ page import="java.util.*"%><% response.setContentType("text/xml");%><?xml version="1.0" ?>
<message>
   <content><%= (new java.util.Date())%></content>
   <reload>1000</reload>
</message>

