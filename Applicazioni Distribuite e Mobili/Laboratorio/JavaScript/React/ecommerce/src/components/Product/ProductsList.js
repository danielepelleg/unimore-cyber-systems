import React, { Component } from "react";
import { List } from "@material-ui/core";
import { ProductItem } from "./ProductItem";

export class ProductList extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: this.props.error,
      isLoaded: this.props.isLoaded,
      productsData: this.props.list,
    };
  }

  static getDerivedStateFromProps(props, state) {
    if (props !== state) {
      console.log("List child updating props...");
      return {
        error: props.error,
        isLoaded: props.isLoaded,
        productsData: props.list,
      };
    }
    return null;
  }

  deleteElement(id) {
    this.props.deleteElement(id);
  }

  render() {
    const { error, isLoaded, productsData } = this.state;
    if (error) {
      return <div>Error: {error.message}</div>;
    } else if (!isLoaded) {
      return <div className="products-list">Loading...</div>;
    } else {
      return (
        <div className="products-list">
          <h2>{this.props.listName} List</h2>
          <div>
            <List sx={{ width: "100%" }}>
              {productsData ? (
                productsData.map((product) => {
                  return (
                    <ProductItem
                      key={product.id}
                      product={product}
                      deleteElement={() => this.deleteElement(product.id)}
                    />
                  );
                })
              ) : (
                <span>Still Loading...</span>
              )}
            </List>
          </div>
        </div>
      );
    }
  }
}
