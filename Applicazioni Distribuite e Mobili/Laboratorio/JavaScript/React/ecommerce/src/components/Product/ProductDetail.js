import React, { Component } from "react";
import Grid from "@material-ui/core/Grid";
import Typography from "@material-ui/core/Typography";
import { ListItem } from "@material-ui/core";

export class ProductDetail extends Component {
  constructor(props) {
    super(props);
    this.state = {
      error: null,
      isLoaded: false,
      element: {},
      url: this.props.url,
    };
  }

  componentDidMount() {
    // Retrieve the Element
    fetch(this.state.url)
      .then((res) => res.json())
      .then(
        (result) => {
          this.setState({
            isLoaded: true,
            element: result,
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
    // DELETE request using fetch()
    fetch(this.state.url, {
      // Adding method type
      method: "DELETE",
    })
      // Read Response
      .then(
        (response) => {
          if (response.ok) {
            console.log(response);
            this.setState({ element: {}, isLoaded: false });
          }
        },
        (error) => console.log(error)
      );
  };

  render() {
    const { element } = this.state;
    return (
      <div key={element.id}>
        <h3>{element.created_at}</h3>
        <ListItem key={element.id}>
          <Grid container spacing={2}>
            {Object.keys(element).map((key, i) => (
              <Grid item xs={2} key={key}>
                {key[0].toUpperCase() + key.slice(1)}
                <Typography>
                  {Array.isArray(element[key]) ? '' : element[key]}
                  {key === "total_cost"  ? "€" : ""}
                </Typography>
              </Grid>
            ))}
          </Grid>
          <button onClick={() => this.deleteElement(element.id)}>DELETE</button>
        </ListItem>
      </div>
    );
  }
}
