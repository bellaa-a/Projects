import { atom } from "nanostores";
import { PostType } from "@/data/types";

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

  export function toggleAddPost() {

  }

export const $posts = atom<PostType[]>([]);