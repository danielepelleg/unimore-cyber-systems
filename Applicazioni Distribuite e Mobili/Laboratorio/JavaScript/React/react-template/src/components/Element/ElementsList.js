import React, { Component } from "react";
import { List } from "@material-ui/core";
import { ElementItem } from "./ElementItem";

export class ElementList extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: this.props.error,
      isLoaded: this.props.isLoaded,
      elementsList: this.props.list,
    };
  }

  static getDerivedStateFromProps(props, state) {
    if (props !== state) {
      console.log("List child updating props...");
      return {
        error: props.error,
        isLoaded: props.isLoaded,
        elementsList: props.list,
      };
    }
    return null;
  }

  deleteElement(id) {
    this.props.deleteElement(id);
  }

  render() {
    const { error, isLoaded, elementsList } = this.state;
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
              {elementsList ? (
                elementsList.map((element) => {
                  return (
                    <ElementItem
                      key={element.id}
                      element={element}
                      deleteElement={() => this.deleteElement(element.id)}
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
