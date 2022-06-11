import React from "react";
import Grid from "@material-ui/core/Grid";
import Typography from "@material-ui/core/Typography";
import { ListItem } from "@material-ui/core";

export function ProductItem(props) {
  const element = props.product;
  delete element.image;

  return (
    <div key={props.id}>
      <h3>{props.title}</h3>
      <ListItem key={props.id}>
        <Grid container spacing={2}>
          {Object.keys(element).map((key, i) => (
            <Grid item xs={2} key={key}>
              {key[0].toUpperCase() + key.slice(1)}
              <Typography>
                {element[key]}
                {key === "price" ? "€" : ""}
              </Typography>
            </Grid>
          ))}
        </Grid>
        <button onClick={() => props.deleteElement(element.id)}>DELETE</button>
      </ListItem>
    </div>
  );
}
