function makeHttpRequest(url, callback_function, return_xml)
{
   var http_request = false;
   if (window.XMLHttpRequest) { // Mozilla, Safari,...
       http_request = new XMLHttpRequest();
       if (http_request.overrideMimeType) {
           http_request.overrideMimeType('text/xml');
       }

   } else if (window.ActiveXObject) { // IE
       try {
           http_request = new ActiveXObject("Msxml2.XMLHTTP");
       } catch (e) {
           try {
               http_request = new ActiveXObject("Microsoft.XMLHTTP");
           } catch (e) {}
       }
   }

   if (!http_request) {
       alert('Browser doesn\'t support Ajax');
       return false;
   }
   http_request.onreadystatechange = function() {
       if (http_request.readyState == 4) {
           if (http_request.status == 200) {
               if (return_xml) {
                   eval(callback_function + '(http_request.responseXML)');
               } else {
                   eval(callback_function + '(http_request.responseText)');
               }
           } else {
               alert('There was a problem with the request.(Code: ' + http_request.status + ')');
           }
       }
   }
   http_request.open('GET', url, true);
   http_request.send(null);
}

function loadXML(xml)
{
    var content = xml.getElementsByTagName('sessionID').item(0).firstChild.nodeValue;
    document.getElementById('sessionID').innerHTML = content;
    content = xml.getElementsByTagName('creationDate').item(0).firstChild.nodeValue;
    document.getElementById('creationDate').innerHTML = content;
    content = xml.getElementsByTagName('accessDate').item(0).firstChild.nodeValue;
    document.getElementById('accessDate').innerHTML = content;
    content = xml.getElementsByTagName('counter').item(0).firstChild.nodeValue;
    document.getElementById('counter').innerHTML = content;
}

function nextPhrase(arg)
{
    var url = 'SessionServerSide.jsp?go=' + arg;   
    makeHttpRequest(url, 'loadXML', true);
}

window.onload = nextPhrase('none');
