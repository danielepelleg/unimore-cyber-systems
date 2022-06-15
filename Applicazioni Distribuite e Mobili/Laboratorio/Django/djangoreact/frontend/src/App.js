import React from 'react';
import RouteRules from './RouteRules';

function App() {
  return (
    <div className="App">
      <RouteRules />
      <a href="{% url 'products:index' %}">Back to Index</a>
    </div>
  );
}

export default App;
