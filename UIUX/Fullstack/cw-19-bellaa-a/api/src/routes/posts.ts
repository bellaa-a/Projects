import { Hono } from "hono";
import { db } from "../db";
import { posts } from "../db/schema";
import { eq } from "drizzle-orm/sql"

const postsRoute = new Hono();

// Read all posts
postsRoute.get("/posts", async (c) => {
  try {
    const allPosts = await db.select().from(posts);
    return c.json(allPosts);
  } catch (err) {
    console.log(err);
    return c.json({ error: "Failed to fetch posts" }, 500);
  }
});

// Read a specific post
postsRoute.get("/posts/:id", async (c) => {
  try {
    const id = Number(c.req.param("id"));
    const post = await db
      .select()
      .from(posts)
      .where(eq(posts.id, id))
      .get()
    // SELECT * FROM posts WHERE id = :id

    if (!post) {
      return c.json({ error: "Post not found" }, 404);
    }      

    return c.json(post);
  } catch (err) {
    console.log(err);
    return c.json({ error: "Failed to fetch then post" }, 500);
  }
});

// Delete a post
postsRoute.delete("/posts/:id", async (c) => {
  try {
    const id = Number(c.req.param("id"));
    const post = await db
      .delete(posts)
      .where(eq(posts.id, id))
      .returning()
      .get()

    if (!post) {
      return c.json({ error: "Post not found" }, 404);
    }      

    return c.json(post);
  } catch (err) {
    console.log(err);
    return c.json({ error: "Failed to fetch then post" }, 500);
  }
});

postsRoute.post("/posts", async (c) => {
  try {
    const { content } = await c.req.json();
    const post = await db
      .insert(posts)
      .values({
        content,
        date: new Date()
      })
      .returning()
      .get()      

    return c.json(post);
  } catch (err) {
    console.log(err);
    return c.json({ error: "Failed to fetch then post" }, 500);
  }
});

// Update a post
postsRoute.patch("/posts/:id", async (c) => {
  try {
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
      return c.json({ error: "Post not found" }, 404);
    }      

    return c.json(post);
  } catch (err) {
    console.log(err);
    return c.json({ error: "Failed to update the post" }, 500);
  }
});

postsRoute.onError((err, c) => {
  console.log(err);
  return c.json({ error: "An unexpected error happened!" }, 500);
})

export default postsRoute;