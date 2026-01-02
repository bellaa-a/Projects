import { Hono } from "hono";
import { db } from "../db";
import { comments } from "../db/schema";
import { eq, asc, desc, like, count, SQL, and } from "drizzle-orm";
import {
  createCommentSchema,
  updateCommentSchema,
  getCommentSchema,
  queryParamsSchema,
  getCommentsSchema,
} from "../validators/schemas";
import { zValidator } from "@hono/zod-validator";
import { HTTPException } from "hono/http-exception";

const commentRoutes = new Hono();

// Get all comments for a post
commentRoutes.get(
  "/posts/:postId/comments",
  zValidator("param", getCommentsSchema),
  zValidator("query", queryParamsSchema),
  async (c) => {
    const { postId } = c.req.valid("param");
    const { sort, search, page = 1, limit = 10 } = c.req.valid("query");

    const whereClause: (SQL | undefined)[] = [];
    whereClause.push(eq(comments.postId, postId));
    if (search) {
      whereClause.push(like(comments.content, `%${search}%`));
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
        .select()
        .from(comments)
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
  zValidator("param", getCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const comment = await db
      .select()
      .from(comments)
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
  zValidator("param", getCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const deletedComment = await db
      .delete(comments)
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .returning()
      .get();
    if (!deletedComment) {
      throw new HTTPException(404, { message: "Comment not found" });
    }
    return c.json(deletedComment);
  },
);

// Create a new comment for a post
commentRoutes.post(
  "/posts/:postId/comments",
  zValidator("param", getCommentsSchema),
  zValidator("json", createCommentSchema),
  async (c) => {
    const { postId } = c.req.valid("param");
    const { content } = c.req.valid("json");
    const newComment = await db
      .insert(comments)
      .values({
        content,
        date: new Date(),
        postId,
      })
      .returning()
      .get();

    return c.json(newComment);
  },
);

// Update a comment by id for a post
commentRoutes.patch(
  "/posts/:postId/comments/:commentId",
  zValidator("param", getCommentSchema),
  zValidator("json", updateCommentSchema),
  async (c) => {
    const { postId, commentId } = c.req.valid("param");
    const { content } = c.req.valid("json");
    const updatedComment = await db
      .update(comments)
      .set({ content })
      .where(and(eq(comments.id, commentId), eq(comments.postId, postId)))
      .returning()
      .get();

    if (!updatedComment) {
      throw new HTTPException(404, { message: "Comment not found" });
    }
    return c.json(updatedComment);
  },
);

export default commentRoutes;
