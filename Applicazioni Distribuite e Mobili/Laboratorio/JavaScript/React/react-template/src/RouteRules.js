import React from "react";
import { BrowserRouter as Router, Switch, Route, Link } from "react-router-dom";
import { Elements, ElementDetail } from "./components";

export default function RouteRules(props) {
  return (
    <Router>
      <div>
        <ul>
          <li>
            <Link to="/">Home</Link>
          </li>
          <li>
            <Link to="/list">List</Link>
          </li>
          <li>
            <Link to="/cart">cart</Link>
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
          <Route path="/list">
            <Elements
              listName="Elements"
              itemName="Element"
              url=""
            />
          </Route>
          <Route path="/cart">
            <ElementDetail url="" />
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
