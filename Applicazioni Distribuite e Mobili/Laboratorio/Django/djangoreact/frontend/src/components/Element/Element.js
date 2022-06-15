import React, { Component } from "react";
import { ElementList } from "./ElementsList";

export default class Element extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: null,
      isLoaded: false,
      elementsList: [],
      url: this.props.url,
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
            elementsList: result,
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
            const newElementsList = this.state.elementsList.filter(
              (element) => element.id !== id
            );
            this.setState({ elementsList: newElementsList });
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
        this.setState({ elementsList: [...this.state.elementsList, json] })
      );
  };

  render() {
    const { error, isLoaded, elementsList } = this.state;
    return (
      <div className="element">
        <ElementList
          listName={this.props.listName}
          error={error}
          isLoaded={isLoaded}
          list={elementsList}
          deleteElement={this.deleteElement}
        />
      </div>
    );
  }
}
