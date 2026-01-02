import { Hono } from "hono";
import postRoutes from "./routes/posts";
import { HTTPException } from "hono/http-exception";
import commentRoutes from "./routes/comments";
import { cors } from "hono/cors";
import authRoutes from "./routes/auth";
import { logger } from "hono/logger";
import type { Context } from "./lib/context.js";
import { auth } from "./middleware/auth";

const app = new Hono<Context>();

app.use(logger());
app.use(
  "/*",
  cors({
    origin: (origin) => origin, // Allow any origin
    credentials: true, // Allow credentials
    allowMethods: ["GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS"],
    allowHeaders: ["Content-Type", "Authorization"],
    exposeHeaders: ["Set-Cookie"],
  }),
);

app.get("/", (c) => {
  return c.text("Hello Hono!");
});

app.use("/*", auth);

app.route("/", authRoutes);
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
