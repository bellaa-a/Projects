import { API_URL } from "../env";
import type { CommentType, PostType } from "./types";

// Fetch all posts
export const fetchPosts = async (
  page: number = 1,
  limit: number = 10,
): Promise<{ data: PostType[]; total: number }> => {
  const response = await fetch(
    `${API_URL}/posts?sort=desc&page=${page}&limit=${limit}`,
  );
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const { data, total }: { data: PostType[]; total: number } =
    await response.json();
  return { data, total };
};

// Delete a post by id
export const deletePost = async (id: string): Promise<boolean> => {
  const response = await fetch(`${API_URL}/posts/${id}`, { method: "DELETE" });
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  return true;
};

// Create a post
export const createPost = async (content: string): Promise<PostType> => {
  const response = await fetch(`${API_URL}/posts`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      content,
    }),
  });
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const data: PostType = await response.json();
  return data;
};

// Edit a post
export const editPost = async (
  id: string,
  content: string,
): Promise<PostType> => {
  const response = await fetch(`${API_URL}/posts/${id}`, {
    method: "PATCH",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ content }),
  });
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const data: PostType = await response.json();
  return data;
};

// Fetch all comments for a post
export const fetchComments = async (postId: string): Promise<CommentType[]> => {
  const response = await fetch(`${API_URL}/posts/${postId}/comments?sort=desc`);
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const { data }: { data: CommentType[] } = await response.json();
  return data;
};

// Delete a comment for a post
export const deleteComment = async (
  postId: string,
  commentId: string,
): Promise<boolean> => {
  const response = await fetch(
    `${API_URL}/posts/${postId}/comments/${commentId}`,
    {
      method: "DELETE",
    },
  );
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  return true;
};

// Create a comment for a post
export const createComment = async (
  postId: string,
  content: string,
): Promise<CommentType> => {
  const response = await fetch(`${API_URL}/posts/${postId}/comments`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      content,
    }),
  });
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const data: CommentType = await response.json();
  return data;
};

// Edit a comment for a post
export const editComment = async (
  postId: string,
  commentId: string,
  content: string,
): Promise<CommentType> => {
  const response = await fetch(
    `${API_URL}/posts/${postId}/comments/${commentId}`,
    {
      method: "PATCH",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ content }),
    },
  );
  if (!response.ok) {
    throw new Error(`API request failed! with status: ${response.status}`);
  }
  const data: CommentType = await response.json();
  return data;
};
