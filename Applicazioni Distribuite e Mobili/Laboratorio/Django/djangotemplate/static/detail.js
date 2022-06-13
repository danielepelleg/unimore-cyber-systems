$(initialize);

// GET Detail Object
function initialize() {
  $.ajax({
    dataType: "json",
    url: url,
    success: detailCallback,
  });
}

renderItem = (key, value = null) => {
  item = "";
  item += '<li id="' + key + '">';
  item +=
    "<label> <b>" +
    key.charAt(0).toUpperCase() +
    key.slice(1) +
    ":</b> " +
    (value ? value : "") +
    "</label>";
  item += "</li>";
  return item;
};

renderFieldsList = (object, elementId) => {
  let elementData = '<ul id="' + elementId + '">';
  Object.entries(object).forEach(([key, value]) => {
    // Nested Object Case
    if (typeof value === "object" && value !== null) {
      elementData += renderItem(key);
      elementData += "<ul>";
      Object.entries(value).forEach(([nestedKey, nestedValue]) => {
        elementData += renderItem(nestedKey, nestedValue);
      });
      elementData += "</ul>";
    } 
    else elementData += renderItem(key, value);
  });
  return elementData;
};

detailCallback = (data) => {
  let elementId = data.id;
  let result = renderFieldsList(data, elementId);
  $("#root").append(result);
};
