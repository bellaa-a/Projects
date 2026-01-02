import "../style.css";

// refactoring

// Data (holds the "state" of my application)
let todos = [
    { id: 1, text: "Buy coffee", isCompleted: false },
    { id: 2, text: "Buy sugar", isCompleted: false },
    { id: 3, text: "Buy jam", isCompleted: false }
];

let nextTodoId = 4;

let filter = "all";


function createTodoItemDiv(todo) {
    const todoItemDiv = document.createElement("div");
    todoItemDiv.classList.add("p-4", "todo-item");

    const todoTextDiv = createTodoTextDiv(todo);
    const todoEditInput = createTodoEditInput(todo);

    todoItemDiv.appendChild(todoTextDiv);
    todoItemDiv.appendChild(todoEditInput);

    return todoItemDiv;
}

function createTodoTextDiv(todo) {
    const todoTextDiv = document.createElement("div");
    todoTextDiv.classList.add("todo-text")
    todo.isCompleted && todoTextDiv.classList.add("line-through")
    todoTextDiv.textContent = todo.text;
    todoTextDiv.setAttribute("todo-id", todo.id)
    return todoTextDiv;
}

function createTodoEditInput(todo) {
    const todoEditInput = document.createElement("input");
    todoEditInput.classList.add("hidden", "todo-edit")
    todoEditInput.setAttribute("value", todo.text);
    return todoEditInput;
}

function filterTodo(todo) {
    if (filter === "all") {
        return true;
    } else if (filter === "completed" && todo.isCompleted) {
        return true;
    } else if (filter === "active" && !todo.isCompleted) {
        return true;
    }

    return false;
}

// Functions (classes) that operate on data (update the state or render UI base on state or both)
function renderTodos() {
    document
        .getElementById("todo-list")
        .replaceChildren(...todos
            .filter(filterTodo)
            .map(createTodoItemDiv)
        );
}

document.addEventListener(
    "DOMContentLoaded", 
    renderTodos
);


const navElement = 
    document.getElementById("todo-nav");

function handleClickOnNavBar(event) {
    const target = event.target;
    // const { target } = event;
    if (target.tagName === "A") {
        filter = getFilterFromAnchor(target);
        renderTodos();
        renderNavbar();
    }
}

function getFilterFromAnchor(anchor) {
    const action = anchor.href.split("/").pop();
    const filter = action === "" ? "all" : action;
    return filter 
}

function renderNavbar() {
    const anchors = navElement.children;
    Array.from(anchors).forEach(anchor => 
        anchor
            .classList[
                filter === getFilterFromAnchor(anchor) ? "add" : "remove"
            ](
                "underline",
                "underline-offset-4", 
                "decoration-rose-800", 
                "decoration-2"
            )
    );
}

navElement.addEventListener(
    "click",
    handleClickOnNavBar
)

const todoList = document.getElementById("todo-list");

function handleClickOnTodoList(event) {
    const target = event.target;
    const todoId = target.getAttribute("todo-id")

    todos = todos.map(todo => ({
            ...todo, 
            isCompleted: Number(todoId) === todo.id ? 
                !todo.isCompleted : todo.isCompleted
        })
    )

    renderTodos();
}

todoList.addEventListener("click", handleClickOnTodoList);


const newTodoInput =
    document.getElementById("new-todo");

newTodoInput.addEventListener(
    "keydown",
    handleKeyDownOnTodoInput
)

function handleKeyDownOnTodoInput(event) {
    const key = event.key;
    if (key === "Enter") {
        const todoText = event.target.value;
        const newTodo = {
            id: nextTodoId++,
            text: todoText,
            isCompleted: false
        }
        todos.push(newTodo);
        renderTodos();
        event.target.value = "";
    }
}