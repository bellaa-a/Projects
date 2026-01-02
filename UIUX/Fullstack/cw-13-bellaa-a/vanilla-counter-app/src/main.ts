import "./style.css";

const app = document.getElementById("app");

app!.innerHTML = `
  <div class="control">
    <button id="reset">↻</button>
  </div>
  <div id="counter">0</div>
  <div class="control">
    <button id="decrement">-</button>
    <button id="increment">+</button>
  </div>
`;

let count = 0;

const counter = document.getElementById("counter");
const increment = document.getElementById("increment");
const decrement = document.getElementById("decrement");
const reset = document.getElementById("reset");

const updateCounter = (): void => {
  counter!.innerHTML = count.toString();
};

increment!.addEventListener("click", () => {
  count++;
  updateCounter();
});

decrement!.addEventListener("click", () => {
  count--;
  updateCounter();
});

reset!.addEventListener("click", () => {
  count = 0;
  updateCounter();
});
