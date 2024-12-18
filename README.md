# **Optimal Pairing System**

- **Author:** [@marinananias](https://www.github.com/marinananias)
- **Final Project:** CSC 213 - Operating Systems & Parallel Algorithms

## **Overview**

The Optimal Pairing System is a C application designed to match entities (mentees and mentors or participants and panels) based on compatibility. It employs computing concepts like multi-threading, synchronization, and file handling.

It has the purpose to be used at BRASA-Brazilian Student Association-to automate the matching process for various organizational needs.

## **Key Features**

- **Multi-threaded Performance**: Parallel processing of compatibility scores for faster execution.
- **Synchronization**: Mutex-based locking ensures data consistency during multi-threaded operations.
- **Capacity Constraints**: Respect mentors' capacity limits during matching.
- **Logging**:
  - Execution performance (threaded vs non-threaded).
  - Evaluated arrangements and their scores.
  - Panel popularity in participant-panel matching.
- **Customizable Inputs**: Handles two different data scenarios with flexible input formats.

## **How to use this program?**

1. **Clone the Repository:**

```bash
git clone https://github.com/marinananias/Optimal-Paring-System.git
cd final_project
```

2. **Build the Project:**

```bash
make
```

3. **Run the Program:**

- `mentee_mentor`:

```bash
./main mentee_mentor inputs/mentees1.csv inputs/mentors1.csv
```

- `participant_panel`:

```bash
./main participant_panel inputs/participants1.csv inputs/panels1.csv
```

## **Command Syntax**

```bash
./main <category> <file1> <file2>
```

- `<category>`:

  - `mentee_mentor`: Match mentees to mentors, respecting capacity constraints.
  - `participant_panel`: Match participants to panels without constraints.

- `<file1>`: CSV file containing the first dataset (mentees or participants).

- `<file2>`: CSV file containing the second dataset (mentors or panels).

## **Input Files Provided**

### **mentees_mentors**

- Small Inputs: `mentees1.csv` and `mentors1.csv`
  Verify that the program runs correctly with minimal data.
- Medium Inputs: `mentees2.csv` and `mentors2.csv`
  Test the system under moderately realistic conditions.
- Large Inputs: `mentees3.csv` and `mentors3.csv`
  Compare execution times of threaded vs. non-threaded implementations for large workloads.

### **participants_panels**

- Small Inputs: `participants1.csv` and `panels1.csv`
  Validate that the system correctly matches participants to panels.
- Medium Inputs: `participants2.csv` and `panels2.csv`
  Test feature completeness under moderately realistic conditions.
- Large Inputs: `participants3.csv` and `panels3.csv`
  Compare execution times of threaded vs. non-threaded implementations for large workloads.

## **Outputs**

The program generates various output files based on the matching category:

- `output.csv:`
  Contains final matches with compatibility scores.

**Example:**

```plaintext
Mentee,Mentor,Compatibility Score
Alice,George,2
Bob,Holly,1
```

- `panel_popularity.csv` (for participant_panel):
  Logs the number of matches for each panel.

**Example:**

```plaintext
Panel,Match Count
Panel A,5
Panel B,3
```

- `arrangement_scores.log`:
  Logs all evaluated arrangements and their total scores for mentee-mentor matching.

**Example:**

```plaintext
Arrangement,Total Score
[0, 1], 3
```

- `threading_performance.log`:
  Logs execution times for threaded and non-threaded operations.

**Example:**

```plaintext
Threaded Execution Time: 0.001221 seconds
Non-Threaded Execution Time: 0.000079 seconds
```

## **Error Handling**

- Invalid or missing files produce detailed error messages.
- Memory allocation failures are handled with error logs.
- Input validation ensures compatibility with expected formats.

## **Known Limitations**

- Assumes all input files are correctly formatted.
- Small datasets may not benefit in time of execution from thread parallelism. May actually be hurt.

## **License**

This project is released under the MIT License.
