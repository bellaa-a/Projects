import { useEffect, useState } from "react";

// Hook
function App() {

  const [count, setCount] = useState<number>(0);
  const [val, setVal] = useState<number>(5);

  const updateCount = (action: "increment" | "decrement" | "reset") => {
    if (action === "increment") {
      setCount(count + 1);
    } else if (action === "decrement") {
      setCount(count - 1);
    } else if (action === "reset") {
      setVal(10);
    }
  }

  // useEffect(() => {
  //   console.log("first render");
  // }, [])

  // useEffect(() => {
  //   console.log("render");
  // })

  useEffect(() => {
    console.log("count is", count);
  }, [count])

  useEffect(() => {
    console.log("val is", val);
  }, [val])

  return (
    <div>
      <div className="control">
        <button 
          id="reset"
          onClick={
            (event) => { 
              event.preventDefault();
              updateCount("reset") 
            }
          }
        >↻ {val}</button>
      </div>
      <div id="counter">{count}</div>
      <div className="control">
        <button 
          id="decrement"
          onClick={
            (event) => { 
              event.preventDefault();
              updateCount("decrement") 
            }
          }
        >-</button>
        <button 
          id="increment"
          onClick={
            (event) => { 
              event.preventDefault();
              updateCount("increment") 
            }
          }
        >+</button>
      </div>
    </div>
  )
}

export default App
