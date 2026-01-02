import { Hono } from "hono";
import postsRoutes from "./routes/posts.js";

const app = new Hono();

app.get("/", (c) => {
  return c.text("Hello Hono!");
});

app.route("/", postsRoutes);

export default app;
