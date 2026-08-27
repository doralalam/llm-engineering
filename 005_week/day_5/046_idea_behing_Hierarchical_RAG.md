# Hierarchical RAG

**Hierarchical RAG = RAG with information organized into multiple levels.**

The basic idea is:

> **Instead of searching all documents and chunks at the same level, organize information hierarchically and search from broad → specific.**

For example:

```text
Company
   ↓
Department
   ↓
Project
   ↓
Document
   ↓
Section
   ↓
Chunk
```

The system first finds the **right high-level area**, and then searches for the **specific information** inside it.

---

# Architecture

```text
                    Documents
                        ↓
                 Organize / Index
                        ↓
              ┌──────────────────┐
              │   Hierarchy      │
              └────────┬─────────┘
                       ↓
                 Level 1: Topic
                       ↓
                 Level 2: Document
                       ↓
                 Level 3: Section
                       ↓
                 Level 4: Chunk
                       ↓
                  Relevant Context
                       ↓
                       LLM
                       ↓
                    Answer
```

## Simple Example

Suppose a company has thousands of documents.

The hierarchy could be:

```text
Company
│
├── HR
│   ├── Leave Policy
│   ├── Employee Benefits
│   └── Hiring Policy
│
├── Finance
│   ├── Expenses
│   ├── Revenue
│   └── Budget
│
└── Engineering
    ├── API Documentation
    ├── Architecture
    └── Deployment
```

User asks:

> **"What is the employee vacation policy?"**

Instead of searching every chunk:

```text
All Documents
      ↓
Thousands of Chunks
      ↓
Search Everything
```

Hierarchical RAG can search like:

```text
User Query
    ↓
HR
    ↓
Leave Policy
    ↓
Relevant Section
    ↓
Relevant Chunk
    ↓
LLM
    ↓
Answer
```

---

# Traditional RAG vs Hierarchical RAG

### Traditional RAG

```text
Question
   ↓
Search all chunks
   ↓
Top-K chunks
   ↓
LLM
   ↓
Answer
```

### Hierarchical RAG

```text
Question
   ↓
Find relevant topic
   ↓
Find relevant document
   ↓
Find relevant section
   ↓
Find relevant chunk
   ↓
LLM
   ↓
Answer
```

---

# Key Idea

The main difference is:

```text
Traditional RAG
→ Search chunks directly

Hierarchical RAG
→ Search from broad → specific
```

This can help when you have **large amounts of structured or hierarchical information**.

## In One Sentence

> **Hierarchical RAG organizes knowledge into multiple levels and retrieves information progressively from broad categories down to specific chunks before giving the relevant context to the LLM.**
