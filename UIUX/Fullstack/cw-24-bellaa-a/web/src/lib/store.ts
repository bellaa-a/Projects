import { atom } from "nanostores";
import type { CommentType, PostType } from "@/data/types";

export const $showAddPost = atom(false);

export function toggleAddPost() {
  $showAddPost.set(!$showAddPost.get());
}

export const $posts = atom<PostType[]>([]);

export function setPosts(posts: PostType[]) {
  $posts.set(posts);
}

export function addPost(post: PostType) {
  $posts.set([post, ...$posts.get()]);
}

export function removePost(id: string) {
  $posts.set($posts.get().filter((post) => post.id !== id));
}

export function updatePostContent(id: string, content: string) {
  $posts.set(
    $posts.get().map((post) => {
      if (post.id === id) {
        return { ...post, content: content };
      }
      return post;
    }),
  );
}

export const $comments = atom<CommentType[]>([]);

export function setComments(comments: CommentType[]) {
  $comments.set(comments);
}

export function addComment(comment: CommentType) {
  $comments.set([comment, ...$comments.get()]);
}

export function removeComment(id: string) {
  $comments.set($comments.get().filter((comment) => comment.id !== id));
}

export function updateCommentContent(id: string, content: string) {
  $comments.set(
    $comments.get().map((comment) => {
      if (comment.id === id) {
        return { ...comment, content: content };
      }
      return comment;
    }),
  );
}

export const $showAddComment = atom(false);

export function toggleAddComment() {
  $showAddComment.set(!$showAddComment.get());
}
