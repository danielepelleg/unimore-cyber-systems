import React from "react";
import { BrowserRouter as Router, Switch, Route, Link } from "react-router-dom";
import { Products, ProductDetail } from "./components";

export default function RouteRules(props) {
  return (
    <Router>
      <div>
        <ul>
          <li>
            <Link to="/">Home</Link>
          </li>
          <li>
            <Link to="/products">Products List</Link>
          </li>
          <li>
            <Link to="/cart">User Cart</Link>
          </li>
        </ul>

        <hr />

        {/*
            A <Switch> looks through all its children <Route>
            elements and renders the first one whose path
            matches the current URL. Use a <Switch> any time
            you have multiple routes, but you want only one
            of them to render at a time
        */}
        <Switch>
          <Route exact path="/">
            <Home />
          </Route>
          <Route path="/products">
            <Products
              listName="Products"
              itemName="Product"
              url="http://127.0.0.1:8000/products/api/"
            />
          </Route>
          <Route path="/cart">
            <ProductDetail url="http://localhost:8000/api/cart/1" />
          </Route>
        </Switch>
      </div>
    </Router>
  );
}

function Home() {
  return (
    <div>
      <h2>Home</h2>
    </div>
  );
}
