import { Hono } from "hono";
import { db } from "../db";
import { comments, posts, users } from "../db/schema";
import { eq, asc, desc, like, count, SQL, and } from "drizzle-orm";
import {
  createCommentSchema,
  updateCommentSchema,
  getCommentSchema,
  queryParamsSchema,
  getCommentsSchema,
} from "../validators/schemas";
import { zValidator } from "@hono/zod-validator";
import type { Context } from "../lib/context.js";
import { authGuard } from "../middleware/auth-guard";
import { HTTPException } from "hono/http-exception";

const commentRoutes = new Hono<Context>();

// Get all comments for a post
commentRoutes.get(
  "/posts/:postId/comments",
  authGuard,
  zValidator("param", getCommentsSchema),
  zValidator("query", queryParamsSchema),
  async (c) => {
    const { postId } = c.req.valid("param");
    const {
      sort,
      search,
      page = 1,
      limit = 10,
      username,
    } = c.req.valid("query");

    const whereClause: (SQL | undefined)[] = [];
    whereClause.push(eq(comments.postId, postId));
    if (search) {
      whereClause.push(like(comments.content, `%${search}%`));
    }
    if (username) {
      const user = await db
        .select()
        .from(users)
        .where(eq(users.username, username))
        .get();

      if (!user) {
        throw new HTTPException(404, { message: "User not found" });
      }

      whereClause.push(eq(comments.userId, user.id));
    }

    const orderByClause: SQL[] = [];
    if (sort === "desc") {
      orderByClause.push(desc(comments.date));
    } else if (sort === "asc") {
      orderByClause.push(asc(comments.date));
    }

    const offset = (page - 1) * limit;

    const [allComments, [{ totalCount }]] = await Promise.all([
      db
        .select({
          id: comments.id,
          content: comments.content,
          date: comments.date,
          author: {
            id: users.id,
            name: users.name,
            username: users.username,
          },
        })
        .from(comments)
        .leftJoin(users, eq(comments.userId, users.id))
        .where(and(...whereClause))
        .orderBy(...orderByClause)
        .limit(limit)
        .offset(offset),
      db
        .select({ totalCount: count() })
        .from(comments)
        .where(and(...whereClause)),
    ]);

    return c.json({
      data: allComments,
      page,
      limit,
      total: totalCount,
    });
  },
);

// Get a single comment by id for a post
commentRoutes.get(
  "/posts/:postId/comments/:commentId",
  authGuard,
  zValidator("param", getCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const comment = await db
      .select({
        id: comments.id,
        content: comments.content,
        date: comments.date,
        author: {
          id: users.id,
          name: users.name,
          username: users.username,
        },
      })
      .from(comments)
      .leftJoin(users, eq(comments.userId, users.id))
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .get();
    if (!comment) {
      throw new HTTPException(404, { message: "Comment not found" });
    }
    return c.json(comment);
  },
);

// Delete a comment by id for a post
commentRoutes.delete(
  "/posts/:postId/comments/:commentId",
  authGuard,
  zValidator("param", getCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const user = c.get("user");

    const comment = await db
      .select()
      .from(comments)
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .get();

    if (!comment) {
      throw new HTTPException(404, { message: "Comment not found" });
    }

    if (comment.userId !== user!.id) {
      throw new HTTPException(403, {
        message: "Unauthorized to delete this comment",
      });
    }

    const deletedComment = await db
      .delete(comments)
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .returning()
      .get();

    return c.json(deletedComment);
  },
);

// Create a new comment for a post
commentRoutes.post(
  "/posts/:postId/comments",
  authGuard,
  zValidator("param", getCommentsSchema),
  zValidator("json", createCommentSchema),
  async (c) => {
    const { postId } = c.req.valid("param");
    const { content } = c.req.valid("json");
    const user = c.get("user"); // Get the authenticated user from the context

    // Check if the post exists
    const post = await db
      .select()
      .from(posts)
      .where(eq(posts.id, postId))
      .get();
    if (!post) {
      throw new HTTPException(404, { message: "Post not found" });
    }

    const newComment = await db
      .insert(comments)
      .values({
        content,
        date: new Date(),
        postId,
        userId: user!.id, // Associate the comment with the authenticated user
      })
      .returning()
      .get();

    return c.json(newComment);
  },
);

// Update a comment by id for a post
commentRoutes.patch(
  "/posts/:postId/comments/:commentId",
  authGuard,
  zValidator("param", getCommentSchema),
  zValidator("json", updateCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const { content } = c.req.valid("json");
    const user = c.get("user");

    const comment = await db
      .select()
      .from(comments)
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .get();

    if (!comment) {
      throw new HTTPException(404, { message: "Comment not found" });
    }

    if (comment.userId !== user!.id) {
      throw new HTTPException(403, {
        message: "Unauthorized to update this comment",
      });
    }

    const updatedComment = await db
      .update(comments)
      .set({ content })
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .returning()
      .get();

    return c.json(updatedComment);
  },
);

export default commentRoutes;
