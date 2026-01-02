import { BASE_URL } from "@/env";
import { createRouter } from "@nanostores/router";

export const $router = createRouter({
  home: `${BASE_URL}`, // Home page with a list of posts
  post: `${BASE_URL}posts/:postId`, // Post page with a list of comments
  login: `${BASE_URL}login`, // Login page
  register: `${BASE_URL}register`, // Register page
});
