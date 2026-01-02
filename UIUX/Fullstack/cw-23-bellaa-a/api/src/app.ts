import { Hono } from "hono";
import postRoutes from "./routes/posts";
import { HTTPException } from "hono/http-exception";
import commentRoutes from "./routes/comments";
import { cors } from "hono/cors";

const app = new Hono();

app.use("/*", cors());

app.get("/", (c) => {
  return c.text("Hello Hono!");
});

app.route("/", postRoutes);
app.route("/", commentRoutes);

app.onError((err, c) => {
  console.error(`${err}`);

  if (err instanceof HTTPException) {
    return err.getResponse();
  }

  return c.json({ message: "An unexpected error occurred!" }, 500);
});

export default app;
