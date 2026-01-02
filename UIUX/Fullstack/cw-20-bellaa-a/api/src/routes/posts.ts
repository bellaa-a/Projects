import { Hono } from "hono";
import { db } from "../db";
import { posts } from "../db/schema";
import { eq } from "drizzle-orm/sql"
import { HTTPException } from "hono/http-exception";
import { z } from "zod";

const postsRoute = new Hono();

// Read all posts
postsRoute.get("/posts", async (c) => {
  const allPosts = await db.select().from(posts);
  return c.json(allPosts);
});

// Read a specific post
postsRoute.get("/posts/:id", async (c) => {
  const id = Number(c.req.param("id"));
  const post = await db
    .select()
    .from(posts)
    .where(eq(posts.id, id))
    .get()
  // SELECT * FROM posts WHERE id = :id

  if (!post) {
    throw new HTTPException(404, {
      message: "Post not found"
    })
  }      

  return c.json(post);
});

// Delete a post
postsRoute.delete("/posts/:id", async (c) => {
  const id = Number(c.req.param("id"));
  const post = await db
    .delete(posts)
    .where(eq(posts.id, id))
    .returning()
    .get()

  if (!post) {
    throw new HTTPException(404, {
      message: "Post not found"
    })
  }  

  return c.json(post);
});

// Create a new post
postsRoute.post("/posts", 
  async (c, next) => { // middleware for input validation
    const createPostObject = await c.req.json();

    const createPostSchema = z.object({
      content: z
        .string()
        .min(1, "Content cannot be empty")
        .max(240, "Content is too long; it needs to be at most 240 characters!")
    })

    const validationResult = createPostSchema.safeParse(createPostObject);

    if (!validationResult.success) {
      throw new HTTPException(400, {
        message: "Your post needs to include a content attribute"
      })
    }

    await next();
  },
  async (c) => { // request handler (also a middleware)

    const { content } = await c.req.json();

    const post = await db
      .insert(posts)
      .values({
        content,
        date: new Date()
      })
      .returning()
      .get()      

    return c.json(post, 201);
  }
);

// Update a post
postsRoute.patch("/posts/:id", async (c) => {
  const id = Number(c.req.param("id"));
  const { content } = await c.req.json();
  const post = await db
    .update(posts)
    .set({
      content
    })
    .where(eq(posts.id, id))
    .returning()
    .get()

  if (!post) {
    throw new HTTPException(404, {
      message: "Post not found"
    })
  }     

  return c.json(post);
});

export default postsRoute;
