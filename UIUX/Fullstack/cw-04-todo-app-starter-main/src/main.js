import "../style.css";

const todos = [
    "Buy coffee",
    "Buy sugar",
    "Buy jam"
];

// Functions (classes) that operate on data (update the state or render UI base on state or )
function renderTodos() {
    const todoListSection = 
        document.getElementById(
            "todo-list"
        );

    todoListSection.innerHTML = ""; // gets rid of the two hardcoded todo items

    for (let i = 0;
        i < todos.length;
        i++
    ) {
        const todo = todos[i];

        const todoItemDiv = 
            document.createElement("div");
        // todoItemDiv.textContent = todo;
        todoItemDiv.classList.add("p-4", "todo-item"); // matching style of previous todos 

        const todoTextDiv = 
            document.createElement("div");
            todoTextDiv.classList.add("todo-text");
            todoTextDiv.textContent = todo;

        const todoEditInput =
            document.createElement("input");
            todoEditInput.classList.add("hidden", "todo-edit");
        todoEditInput.textContent = todo;
        todoEditInput.setAttribute("value", todo);

        todoItemDiv.appendChild(todoTextDiv);
        todoItemDiv.appendChild(todoEditInput);
        todoListSection.appendChild(todoItemDiv);
    }
}

// renderTodos();

document.addEventListener("DOMContentLoaded", renderTodos)