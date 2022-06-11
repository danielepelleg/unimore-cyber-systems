import React, { Component } from "react";
import { ProductList } from "./ProductsList";
import { ProductForm } from "./ProductForm";

export default class Product extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: null,
      isLoaded: false,
      productsData: [],
      url: "http://127.0.0.1:8000/products/api/",
    };
  }

  componentDidMount() {
    // Retrieve all Elements
    fetch(this.state.url)
      .then((res) => res.json())
      .then(
        (result) => {
          this.setState({
            isLoaded: true,
            productsData: result,
          });
        },
        (error) => {
          this.setState({
            isLoaded: true,
            error,
          });
        }
      );
  }

  deleteElement = (id) => {
    const elementUrl = this.state.url + id;
    console.log(elementUrl);
    // DELETE request using fetch()
    fetch(elementUrl, {
      // Adding method type
      method: "DELETE",
    })
      // Read Response
      .then(
        (response) => {
          if (response.ok) {
            console.log(response);
            const newProductsData = this.state.productsData.filter(
              (product) => product.id !== id
            );
            this.setState({ productsData: newProductsData });
          }
        },
        (error) => console.log(error)
      );
  };

  addElement = (newElement) => {
    // POST request using fetch()
    fetch(this.state.url, {
      // Adding method type
      method: "POST",
      // Adding body or contents to send
      body: JSON.stringify(newElement),
      // Adding headers to the request
      headers: {
        "Content-type": "application/json; charset=UTF-8",
      },
    })
      // Converting to JSON
      .then((response) => {
        if (response.ok) {
          console.log("Success!");
        }
        return response.json();
      })
      // Displaying results to console
      .then((json) =>
        this.setState({ productsData: [...this.state.productsData, json] })
      );
  };

  render() {
    const { error, isLoaded, productsData } = this.state;
    return (
      <div className="products">
        <ProductList
          listName={this.props.listName}
          error={error}
          isLoaded={isLoaded}
          list={productsData}
          deleteElement={this.deleteElement}
        />
        <ProductForm addElement={this.addElement} itemName={this.props.itemName}/>
      </div>
    );
  }
}
