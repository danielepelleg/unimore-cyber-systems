import React, { Component } from "react";
import { List } from "@material-ui/core";
import { ListItem } from "@material-ui/core";

export class ElementForm extends Component {
  constructor(props) {
    super(props);
    this.state = {
      title: "",
      category: "F",
      price: -1,
      pubblication_date: null,
      quantity: -1,
    };
  }

  handleChange = (e) => {
    const targetName = e.target.name;
    this.setState({
      [targetName]: e.target.value,
    });
  }

  onSubmit = (e) => {
    e.preventDefault();
    this.setState({
      pubblication_date: new Date(),
    });
    const newElement = this.state;
    this.props.addElement(newElement);
    e.target.reset();
  }

  render() {
    const categoryOptions = [
      {
        label: "F",
        value: "Food",
      },
      {
        label: "C",
        value: "Clothes",
      },
      {
        label: "T",
        value: "Technology",
      },
      {
        label: "O",
        value: "Other",
      },
    ];

    return (
      <div className="new-form-element">
        <form id="form" onSubmit={this.onSubmit}>
          <h2>Insert New {this.props.itemName}</h2>
          <List>
            <ListItem>
              <span>Category: </span>
              <select
                name="category"
                value={this.state.category}
                onChange={this.handleChange}
              >
                {categoryOptions.map((option) => (
                  <option value={option.label} key={option.label}>
                    {option.value}
                  </option>
                ))}
              </select>
              <br />
            </ListItem>
            <ListItem>
              <label>
                <span>Title: </span>
                <input type="text" name="title" onChange={this.handleChange} />
                <br />
              </label>
              <br />
            </ListItem>
            <ListItem>
              <label>
                <span>Price: </span>
                <input
                  type="decimal"
                  name="price"
                  onChange={this.handleChange}
                />
                <br />
              </label>
            </ListItem>
            <label>
              <input
                type="hidden"
                name="pubblication_date"
                value={new Date()}
              />
            </label>
            <ListItem>
              <label>
                <span>Quantity: </span>
                <input
                  type="number"
                  name="quantity"
                  onChange={this.handleChange}
                />
                <br />
              </label>
            </ListItem>
            <ListItem>
              <br />
              <div>
                <button type="submit">Submit</button>
              </div>
            </ListItem>
          </List>
        </form>
      </div>
    );
  }
}
