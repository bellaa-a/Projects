import { Hono } from "hono";

const postsRoutes = new Hono();

type PostType = {
    id: number;
    content: string;
    date: number
}

const posts: PostType[] = [
    {
        id: 1,
        content: "Post 1",
        date: new Date().getTime()
    },
    {
        id: 2,
        content: "Post 2",
        date: new Date().getTime()
    },
    {
        id: 3,
        content: "Post 3",
        date: new Date().getTime()
    }
]

let nextId = 4;

// Get all the posts
postsRoutes.get("/posts", (c) => {
    return c.json(posts);
});

// Get a specific post
postsRoutes.get("/posts/:id", (c) => {
    const id = parseInt(c.req.param("id"));
    const post = posts.find((p) => { return p.id === id })
    if (!post) {
        return c.json({ error: "Post not found" }, 404);
    }
    return c.json(post);
});

// Delete a specific post
postsRoutes.delete("/posts/:id", (c) => {
    const id = parseInt(c.req.param("id"));
    const postIndex = posts.findIndex((p) => { return p.id === id })
    if (postIndex === -1) {
        return c.json({ error: "Post not found" }, 404);
    }

    const deletedPost = posts.splice(postIndex, 1)[0];
    return c.json(deletedPost);
});

// Create a post
postsRoutes.post("/posts", async (c) => {
    const { content } = await c.req.json();

    const newPost = {
        id: nextId++,
        content: content,
        date: new Date().getTime()
    }
    posts.push(newPost);

    return c.json({ newPost }, 201);
});

// Update a specific post
postsRoutes.patch("/posts/:id", async (c) => {
    const id = parseInt(c.req.param("id"));
    const { content } = await c.req.json();

    const postIndex = posts.findIndex((p) => { return p.id === id })
    if (postIndex === -1) {
        return c.json({ error: "Post not found" }, 404);
    }

    posts[postIndex].content = content;

    // posts[postIndex] = {
    //   ...posts[postIndex],
    //   content,
    // }

    return c.json(posts[postIndex]);
});

export default postsRoutes;