$(renderForm);

renderForm = (elementId) => {
  let formText = "";
  formText += '<form id="insertForm" method="" action="">';
  // Select Form
  formText += '<select name="" id="option_input">';
  formText += '<option value="" disabled selected>Select your option</option>';
  formText += '<option value="value1">Label1</option>';
  formText += '<option value="value2">Label2</option>';
  formText += "</select>";
  // Text Form
  formText += '<input type="text" id="text_input" name="" placeholder=""/>';
  // Number Input
  formText +=
    '<input type="number" id="number_input"  name="" min=1 value="" max="">';
  formText +=
    '\t<input type="button" name="cmd" value="" onclick="submitForm(\'' +
    elementId +
    "','insertForm')\"/>";
  formText += "</form>";
};

submitForm = () => {
  $form = $("#insertForm");
  let data = {
    key: $form.find('select[name="option_input"]').val(),
  };
  let json_data = JSON.stringify(data);
  let requestUrl = "";
  let method = "";
  $.ajax({
    url: requestUrl,
    method: method,
    data: json_data,
    success: updateData,
  });
};

updateData = () => {
  console.log("Success!");
};
