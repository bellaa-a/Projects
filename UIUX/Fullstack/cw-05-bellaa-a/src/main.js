import "../style.css";

// refactoring

// Data (holds the "state" of my application)
const todos = [
    { id: 1, text: "Buy coffee", isCompleted: false },
    { id: 2, text: "Buy sugar", isCompleted: false },
    { id: 3, text: "Buy jam", isCompleted: false }
];

let nextTodoId = 4;

let filter = "all";

// Functions (classes) that operate on data (update the state or render UI base on state or both)
function renderTodos() {
    const todoListSection =
        document.getElementById(
            "todo-list"
        );

    todoListSection.innerHTML = "";

    const filteredTodos = [];

    for(let i = 0;
        i < todos.length;
        i++
    ) {
        const todo = todos[i];

        if (filter === "all") {
            filteredTodos.push(todo);
        } else if (filter === "completed") {
            todo.isCompleted && filteredTodos.push(todo);
        } else if (filter === "active") {
            !todo.isCompleted && filteredTodos.push(todo);
        }

    }

    filteredTodos.forEach(renderFilteredTodo)

    function renderFilteredTodo(todo) {
        const todoItemDiv = 
            document.createElement("div");
        
        todoItemDiv.classList.add("p-4", "todo-item")

        const todoTextDiv = 
            document.createElement("div");
        todoTextDiv.classList.add("todo-text")
        todo.isCompleted && todoTextDiv.classList.add("line-through")
        todoTextDiv.textContent = todo.text;
        todoTextDiv.setAttribute("todo-id", todo.id)

        const todoEditInput = 
            document.createElement("input");
        todoEditInput.classList.add("hidden", "todo-edit")
        todoEditInput.setAttribute("value", todo.text);
        
        todoItemDiv.appendChild(todoTextDiv);
        todoItemDiv.appendChild(todoEditInput);
        todoListSection.appendChild(todoItemDiv)
    }
}

// renderTodos();


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

    for (let i = 0; i < anchors.length; i++) {
        const anchor = anchors[i];
        const anchorFilter = getFilterFromAnchor(anchor);
        if (filter === anchorFilter) {
            anchor.classList.add(
                "underline",
                "underline-offset-4", 
                "decoration-rose-800", 
                "decoration-2"
            )
        } else {
            anchor.classList.remove(
                "underline",
                "underline-offset-4", 
                "decoration-rose-800", 
                "decoration-2"
            )
        }

    }
}

navElement.addEventListener(
    "click",
    handleClickOnNavBar
)

const todoList = document.getElementById("todo-list");

function handleClickOnTodoList(event) {
    const target = event.target;
    const todoId = target.getAttribute("todo-id")
    for(let i = 0;
        i < todos.length;
        i++
    ) {
        const todo = todos[i];
        if (Number(todoId) === todo.id) {
            todo.isCompleted = !todo.isCompleted;
        }
        
    }
    renderTodos();
}

todoList.addEventListener("click", handleClickOnTodoList);