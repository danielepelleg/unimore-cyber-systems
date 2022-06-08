function nextPhrase() {
  // script server-side da invocare
  var url = "DateServerSide.jsp";
  makeHttpRequest(url, "loadXML");
}

/** 
 * main() dello script
 * quando carico la pagina eseguo per la prima volta la funzione nextPhrase();
*/ 
window.onload = nextPhrase();

/**
 * funzione che gestisce i messaggi del server
 */
function loadXML(xml) {
  // html_content è la stringa con l'orario
  var html_content = xml.getElementsByTagName("content").item(0)
    .firstChild.nodeValue;
  // reload after è il timeout per aggiornare il valore
  var reload_after = xml.getElementsByTagName("reload").item(0)
    .firstChild.nodeValue;
  // aggiorna la pagina
  document.getElementById("ajaxResult").innerHTML = html_content;
  // to è l'oggetto timeout usato per la temporizzazione
  try {
  } catch (e) {}
  // impostiamo il prossimo timeout – torniamo a invocare nextPhrase()
  to = setTimeout("nextPhrase()", parseInt(reload_after));
}

/**
 * funzione che interagisce con il server e aggiorna i dati client-side
 */
function makeHttpRequest(url, callback_function) {
  var http_request = false;
  if (window.XMLHttpRequest) {
    // Mozilla, Safari,...
    http_request = new XMLHttpRequest();
    if (http_request.overrideMimeType) {
      http_request.overrideMimeType("text/xml");
    }
  } else if (window.ActiveXObject) {
    // IE
    try {
      http_request = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
      try {
        http_request = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (e) {}
    }
  }
  if (!http_request) {
    alert("Browser doesn't support Ajax");
    return false;
  }
  // prepara la callback per la risposta del server
  http_request.onreadystatechange = function () {
    if (http_request.readyState == 4) {
      if (http_request.status == 200) {
        eval(callback_function + "(http_request.responseXML)");
      } else {
        alert(
          "There was a problem with the request.(Code: " +
            http_request.status +
            ")"
        );
      }
    }
  };
  // prepara la chiamata
  http_request.open("GET", url, true);
  // invia la richiesta al server
  http_request.send(null);
}