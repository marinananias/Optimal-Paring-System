# **Optimal Pairing System**

## **Overview**

The Optimal Pairing System is a C application designed to match entities (mentees and mentors or participants and panels) based on compatibility. It employs computing concepts like multi-threading, synchronization, and file handling.

This program

---

## **Key Features**

- **Multi-threaded Performance**: Parallel processing of compatibility scores for faster execution.
- **Synchronization**: Mutex-based locking ensures data consistency during multi-threaded operations.
- **Capacity Constraints**: Respect mentors' capacity limits during matching.
- **Extensive Logging**:
  - Execution performance (threaded vs non-threaded).
  - Evaluated arrangements and their scores.
  - Panel popularity in participant-panel matching.
- **Customizable Inputs**: Handles various data scenarios with flexible input formats.

---

## **System Requirements**

- **Operating System**: Linux/macOS.
- **Compiler**: GCC (recommended version 9.0+).
- **Dependencies**:
  - POSIX thread library (pthreads).

---

## **Installation and Build**

1. **Clone the Repository**:

```bash
git clone <repository-url>
cd final_project
```

2. **Build the Project:**:

```bash
make
```

3. **Clean Build Files:**:

```bash
make clean
```

---

## **Usage**

# **Command Syntax**:

```bash
./main <category> <file1> <file2>
```

# **Parameters**

### `<category>`:

- `mentee_mentor`: Match mentees to mentors, respecting capacity constraints.
- `participant_panel`: Match participants to panels without constraints.

### `<file1>`:

CSV file containing the first dataset (mentees or participants).

### `<file2>`:

CSV file containing the second dataset (mentors or panels).

---

# **Input File Format**

Input files should follow a structured CSV format.

### **Headers**

For `mentee_mentor`:

```plaintext
Name,Attributes,Capacity
```

For `participant_panel`:

```plaintext
Name,Attributes
```

### **Example: Mentors**

```plaintext
Name,Attributes,Capacity
George,Geography|Math|History,1
Holly,Geography|Chemistry,2
```

### **Example: Mentees**

```plaintext
Name,Attributes
Alice,Math|History
Bob,Chemistry|Physics
```

---

# **Outputs**

The program generates various output files based on the matching category:

### **Output Files**

`output.csv:`
Contains final matches with compatibility scores.

Example:

```plaintext
Mentee/Mentor,Compatibility Score
Alice,George,2
Bob,Holly,1
```

`panel_popularity.csv` (for participant_panel):
Logs the number of matches for each panel.

Example:

```plaintext
Panel,Match Count
Panel A,5
Panel B,3
```

`arrangement_scores.log`:
Logs all evaluated arrangements and their total scores for mentee-mentor matching.
Example:

```plaintext
Arrangement,Total Score
[0, 1], 3
```

`threading_performance.log`:
Logs execution times for threaded and non-threaded operations.
Example:

```plaintext
Threaded Execution Time: 0.001221 seconds
Non-Threaded Execution Time: 0.000079 seconds
```

---

# **Technical Concepts**

1. Parallelism with Threads
   Each thread computes compatibility scores for one individual.
   Reduces computation time by processing multiple mentees/participants simultaneously.
2. Thread Synchronization
   Mutex locks ensure consistency in shared data (compatibility scores).
   Avoids race conditions during multi-threaded updates.
3. File and Data Handling
   Parses CSV input files into structured datasets.
   Writes detailed logs and results for reproducibility.

---

# **Error Handling**

Invalid or missing files produce detailed error messages.
Memory allocation failures are handled with error logs.
Input validation ensures compatibility with expected formats.

---

# **Known Limitations**

Assumes all input files are correctly formatted.
Large datasets may require adjustments in thread pool size for optimal performance.

---

# **License**

This project is released under the MIT License.
