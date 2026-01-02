import Todo from './todo.js';

export const FILTERS = {
  ALL: "all",
  ACTIVE: "active",
  COMPLETED: "completed",
};

class TodoList {
  #todos = [];

  addTodo(text) {
    this.#todos.push(new Todo(text));
  }

  toggleTodoById(id) {
    const todo = this.#todos.find((todo) => todo.id === id);
    if (todo) {
      todo.toggle();
    }
  }

  getTodos(filter) {
    switch (filter) {
      case FILTERS.ALL:
        return this.#todos;
      case FILTERS.COMPLETED:
        return this.#todos.filter((todo) => todo.completed);
      case FILTERS.ACTIVE:
        return this.#todos.filter((todo) => !todo.completed);
    }
  }

  getNumberOfActiveTodos() {
    return this.#todos.reduce((acc, todo) => acc + !todo.completed, 0);
  }

  deleteCompletedTodos() {
    this.#todos = this.#todos.filter((todo) => !todo.completed);
  }

  markAllCompleted() {
    this.#todos.forEach((todo) => {
      todo.completed = true;
    });
  }
}

export default TodoList;