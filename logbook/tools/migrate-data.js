import AWS from 'aws-sdk';

const tableName = "ThermometerLog";
const dbSrc = new AWS.DynamoDB.DocumentClient({ region: 'eu-west-2' });
const dbDest = new AWS.DynamoDB.DocumentClient({ region: 'us-west-1' });

const skip = isNaN(parseInt(process.argv[2])) ? 0 : parseInt(process.argv[2]);

async function main() {

  const query = {
    TableName: tableName
  };

  let logEntries = [];

  do {
    const results = await dbSrc.scan(query).promise();
    logEntries = [...logEntries, ...results.Items];
    query.ExclusiveStartKey = results.LastEvaluatedKey;
  } while (query.ExclusiveStartKey !== undefined);

  const chunk = 25;
  for (let i = skip; i < logEntries.length; i += chunk) {
    const batch = logEntries.slice(i, i+chunk);
   
    console.log("Item " + i + " of " + logEntries.length);

    const batchWriteQuery = {
      RequestItems: {
        "ThermometerLog": batch.map(logEntry => ({
      	  PutRequest: {
            Item: logEntry
          }
        }))
      }
    };
    
    await dbDest.batchWrite(batchWriteQuery).promise();
  }


}

main();
