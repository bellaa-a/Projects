import { PostType } from "@/data/types";
import { API_URL } from "@/env";

const API_ENDPOINT = `${API_URL}/posts`;

// Get posts
export const fetchPosts = async (): Promise<PostType[]> => {
    // to use the fetch function to get all the posts.
    const response = await fetch(`${API_ENDPOINT}`);
    if (!response.ok) {
        throw new Error(`API request failed with status of ${response.status}`);
    }
    const data: PostType[] = await response.json();
    return data;
}

// Create (add) post
export const addPost = async (content: string): Promise<PostType> => {
    const response = await fetch(`${API_ENDPOINT}`, {
        method: "POST",
        body: JSON.stringify({
            content,
            date: new Date().toISOString()
        }),
        headers: {
            "Content-Type": "application/json"
        }
    });
    if (!response.ok) {
        throw new Error(`API request failed with status of ${response.status}`);
    }
    const data: PostType = await response.json();
    return data;
}

// Update post
export const updatePost = async (id: string, content: string): Promise<PostType> => {
    const response = await fetch(`${API_ENDPOINT}/${id}`, {
        method: "PATCH",
        body: JSON.stringify({
            content
        }),
        headers: {
            "Content-Type": "application/json"
        }
    });
    if (!response.ok) {
        throw new Error(`API request failed with status of ${response.status}`);
    }
    const data: PostType = await response.json();
    return data;
}

// Delete post
export const deletePost = async (id: string): Promise<boolean> => {
    const response = await fetch(`${API_ENDPOINT}/${id}`, {
        method: "DELETE",
    });
    if (!response.ok) {
        throw new Error(`API request failed with status of ${response.status}`);
    }
    return true;
}