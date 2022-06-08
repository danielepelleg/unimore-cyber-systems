const cart_url = "/api/cart/" + user_id;

$(initialize);

// GET Cart Object
function initialize() {
    $.ajax({
        dataType: "json",
        url: cart_url,
        success: cartCallback,
    });
}

// Render Cart Page
function cartCallback(data) {
    let nproducts = data.products.length;
    let cartId = data.id;
    let productForm = "<ul id=\""+ cartId +"\">";
    for (let n=0; n < nproducts; n++) {
        productForm += getProductForm(cartId, data.products[n]);
    }
    productForm += "</ul>"
    $("#products").append(productForm);
    addCartInformation(data);
}

// Render Product's Informations
function getProductForm(cartId, product, buttoncommand="edit,delete") {
    let productId = "li_" + product.product.id;
    let productText = "<li id=\""+ productId +"\">";
    productText += "<label>" + product.product.title.toUpperCase() + "</label>";
    productText += "</br></br>"
    productText += "<label>" + "<b>Category</b>: " + product.product.category + "</label>";
    productText += "</br></br>"
    productText += "<label>" + "<b>Unit Price</b>: " + product.product.price + "€" + "</label>";
    productText += "<form id=\"form_" + productId + "\" method=\"\" action=\"\">";
    productText += "<input type=\"hidden\" name=\"id\" value=\""+productId+"\" />";
    productText += "</br>"
    productText += "<label>" + "<b>Chosen Quantity</b>: " + "</label>";
    productText += "<input type=\"number\" name=\"quantity\" min=1 value=\""+ product.quantity + "\" max=\""+ product.product.quantity +"\">"
    // Create 'edit' and 'delete' buttons
    let buttons=buttoncommand.split(",");
    let nb=buttons.length;
    for (let b=0; b < nb; b++) {
        productText += "\t<input type=\"button\" name=\"cmd\" value=\"" + buttons[b] + "\" onclick=\"submitProductForm('"+ cartId +"','form_"+productId+"','"+buttons[b]+"')\" />";
        productText += "</br></br>"
    }
    productText +="</form>";
    productText += "</br></br>"
    productText += "</li>"
    return productText
}

// Handle Cart Actions
function submitProductForm(cartId, formId, command){
    formId_tokens = formId.split("_");
    let productId = formId_tokens[formId_tokens.length - 1]
    let data = {
        product_id: parseInt(productId),
        quantity: parseInt($("#"+formId).children("input[name=quantity]").val())
    };
    let json_data = JSON.stringify(data);
    let request_url = cart_url
    let method = "GET";
    switch(command){
        case "edit":
            method = "POST";
            break;
        case "delete":
            method = "DELETE";
            request_url  =  "/api/cart/" + cartId + "/" + productId
            break;
    }
    $.ajax({
        url: request_url,
        method: method,
        data: json_data,
        success: updateProductForm,
    });
}

// Render new Product Forms
function updateProductForm(data) {
    console.log(data);
    let liId = "";
    let json = JSON.parse(this.data);
    productId = json.product_id;
    // edit total price
    $("#cart_total").html("<b>TOTAL: </b>" + data.total_cost + "€");
    switch(this.method){
        case "DELETE":
            // remove element
            $("#"+"li_"+productId).remove();
            break;
        case "POST":
            break;
    }
    if (data.total_cost == 0){
        submitDeleteCart();
    }
}

// Render Cart Informations
function addCartInformation(cart) {
    const creation_date = new Date(cart.created_at)
    let cartInformation = "<label id=\"cart_total\">"+ "<b>Creation Date: </b>" + creation_date.toLocaleString() + "</label>";
    cartInformation += "</br></br>"
    cartInformation += "<label id=\"cart_total\">"+ "<b>TOTAL: </b>" + cart.total_cost + "€" + "</label>";
    cartInformation += "<div></br>"
    cartInformation += "\t<input type=\"button\" name=\"delete\" value=\" Empty Cart \" onclick=\"submitDeleteCart()\" />";
    cartInformation += "</div>"
    $("#products").append(cartInformation);
}

// DELETE Cart Object
function submitDeleteCart() {
    $.ajax({
        url: cart_url,
        method: "DELETE",
        processData: false,
        success: eraseAll
    });
}

// Delete all Cart data in the page
function eraseAll(data){
    $("#message").append("This cart has been deleted. Once you update this page, you won't see this message anymore.")
    $("#products").remove()
}