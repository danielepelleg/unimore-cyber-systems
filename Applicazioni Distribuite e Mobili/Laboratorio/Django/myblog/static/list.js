$(initialize);

// GET List Objects
function initialize() {
  $.ajax({
    dataType: "json",
    url: url,
    success: listCallback,
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

renderFieldsList = (list) => {
  elementData = "";
  $.each(list, function (index, element) {
    elementData += "<label>" + index + "</label>";
    Object.entries(element).forEach(([key, value]) => {
      // Nested Object Case
      if (typeof value === "object" && value !== null) {
        elementData += renderItem(key);
        elementData += "<ul>";
        Object.entries(value).forEach(([nestedKey, nestedValue]) => {
          elementData += renderItem(nestedKey, nestedValue);
        });
        elementData += "</ul>";
      } else elementData += renderItem(key, value);
    });
  });
  return elementData;
};

listCallback = (data) => {
  console.log(data);
  let result = "";
  console.log(data);
  if (Array.isArray(data) && data) {
    result = renderFieldsList(data);
  }
  $("#root").append(result);
};
