import "../style.css";

import TodoList, { FILTERS } from "./todo-list";

class TodoApp {
  #todoList;
  #filter;
  #todoListElement;
  #activeTodosCount;
  #todoNav;
  #inputNewTodo;
  #markAllCompleted;
  #clearCompleted;

  constructor() {
    this.#todoList = new TodoList();
    this.#filter = FILTERS.ALL;
    this.#todoListElement = document.getElementById("todo-list");
    this.#activeTodosCount = document.getElementById("todo-count");
    this.#todoNav = document.getElementById("todo-nav");
    this.#inputNewTodo = document.getElementById("new-todo");
    this.#markAllCompleted = document.getElementById("mark-all-completed");
    this.#clearCompleted = document.getElementById("clear-completed");

    this.#todoListElement.addEventListener(
      "click",
      this.#handleClickOnTodoList.bind(this),
    );
    this.#inputNewTodo.addEventListener(
      "keydown",
      this.#handleKeyDownToCreateNewTodo.bind(this),
    );
    this.#todoNav.addEventListener(
      "click",
      this.#handleClickOnNavbar.bind(this),
    );
    this.#markAllCompleted.addEventListener(
      "click",
      this.#handleMarkAllCompleted.bind(this),
    );
    this.#clearCompleted.addEventListener(
      "click",
      this.#clearCompletedTodos.bind(this),
    );

    document.addEventListener("DOMContentLoaded", this.#renderTodos.bind(this));
  }

  // Helper function to create todo text element
  #createTodoText(todo) {
    const todoText = document.createElement("div");
    todoText.id = `todo-text-${todo.id}`;
    todoText.classList.add(
      "todo-text",
      ...(todo.completed ? ["line-through"] : []),
    );
    todoText.innerText = todo.text;
    return todoText;
  }

  // Helper function to create todo edit input element
  #createTodoEditInput(todo) {
    const todoEdit = document.createElement("input");
    todoEdit.classList.add("hidden", "todo-edit");
    todoEdit.value = todo.text;
    return todoEdit;
  }

  // Helper function to create a todo item
  #createTodoItem(todo) {
    const todoItem = document.createElement("div");
    todoItem.classList.add("p-4", "todo-item");
    const todoText = this.#createTodoText(todo);
    const todoEdit = this.#createTodoEditInput(todo);
    todoItem.append(todoText, todoEdit);
    return todoItem;
  }

  // Function to render the todos based on the current filter
  #renderTodos() {
    this.#todoListElement.innerHTML = ""; // Clear the current list to avoid duplicates

    const todoElements = this.#todoList
      .getTodos(this.#filter)
      .map(this.#createTodoItem.bind(this));
    this.#todoListElement.append(...todoElements);

    this.#activeTodosCount.innerText = `${this.#todoList.getNumberOfActiveTodos()} item${this.#todoList.getNumberOfActiveTodos() === 1 ? "" : "s"} left`;
  }

  // Event handler to create a new todo item
  #handleKeyDownToCreateNewTodo(event) {
    if (event.key === "Enter") {
      const todoText = event.target.value.trim();
      if (todoText) {
        this.#todoList.addTodo(todoText);
        event.target.value = ""; // Clear the input
        this.#renderTodos();
      }
    }
  }

  // Helper function to find the target todo element
  #findTargetTodoElement = (event) =>
    event.target.id?.includes("todo-text") ? event.target : null;

  // Helper function to parse the todo id from the todo element
  #parseTodoId = (todo) => (todo ? Number(todo.id.split("-").pop()) : -1);

  // Event handler to toggle the completed status of a todo item
  #handleClickOnTodoList = (event) => {
    const todoId = this.#findTargetTodoElement(event);
    const todoIdNumber = this.#parseTodoId(todoId);
    this.#todoList.toggleTodoById(todoIdNumber);
    this.#renderTodos();
  };

  // Helper function to update the class list of a navbar element
  #updateClassList(element, isActive) {
    const classes = [
      "underline",
      "underline-offset-4",
      "decoration-rose-800",
      "decoration-2",
    ];
    if (isActive) {
      element.classList.add(...classes);
    } else {
      element.classList.remove(...classes);
    }
  }

  // Helper function to render the navbar anchor elements
  #renderTodoNavBar(href) {
    Array.from(this.#todoNav.children).forEach((element) => {
      this.#updateClassList(element, element.href === href);
    });
  }

  // Event handler to filter the todos based on the navbar selection
  #handleClickOnNavbar(event) {
    // if the clicked element is an anchor tag
    if (event.target.tagName === "A") {
      const hrefValue = event.target.href;
      this.#filter = hrefValue.split("/").pop() || "all";
      this.#renderTodoNavBar(hrefValue);
      this.#renderTodos();
    }
  }

  // Event handler to mark all todos as completed
  #handleMarkAllCompleted() {
    this.#todoList.markAllCompleted();
    this.#renderTodos();
  }

  // Event handler to clear all completed todos
  #clearCompletedTodos() {
    this.#todoList.deleteCompletedTodos();
    this.#renderTodos();
  }
}

new TodoApp();